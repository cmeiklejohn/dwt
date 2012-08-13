#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#define WIDTH 77
#define HEIGHT 23

const char TASK_LIST_FILE[] = "tasks.txt";

FILE* open_task_list(char *mode_string) {
  FILE *task_list;

  task_list = fopen(TASK_LIST_FILE, mode_string);

  if(task_list == NULL) {
    fprintf(stderr, "Can't open task list: %s\n", TASK_LIST_FILE);
  }

  return task_list;
}

int close_task_list(FILE *task_list) {
  fclose(task_list);

  return 1;
}

int create_task(char *task, FILE *task_list) {
  if((task_list = open_task_list("a"))) {
    fputs(task, task_list);
    fputc('\n', task_list);

    close_task_list(task_list);

    return 1;
  } else {
    return 0;
  }
}

int read_tasks(char *tasks[], FILE *task_list) {
  int i = 0;
  char *task;
  char buffer[100];

  if((task_list = open_task_list("r"))) {
    while(fgets(buffer, sizeof(buffer), task_list) != NULL) {
      if((task = malloc(sizeof(char) * 100)) != NULL ) {
        strncpy(task, buffer, strlen(buffer) - 1);
        tasks[i] = task;
        i++;
      }
    }

    close_task_list(task_list);

    return i;
  } else {
    return 0;
  }
}

int delete_task(char *task, FILE *task_list) {
  int i, task_list_size;
  char *tasks[100];

  if((task_list_size = read_tasks(tasks, task_list))) {
    if((task_list = open_task_list("w"))) {
      for(i = 0; i < task_list_size; i++) {
        if(strncmp(task, tasks[i], sizeof(tasks[i])) != 0) {
          fputs(tasks[i], task_list);
          fputc('\n', task_list);
        }
      }

      close_task_list(task_list);
    } else {
      return 1;
    }

    return 1;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {
  int i, option, task_list_size;
  int cflag = 0, lflag = 0, dflag = 0, iflag = 0;
  char *cvalue, *dvalue;
  char *tasks[100];
  FILE *task_list;

  while((option = getopt(argc, argv, "ilc:d:")) != -1) {
    switch(option) {
    case 'i':
      iflag = 1;
      break;
    case 'c':
      cflag = 1;
      cvalue = optarg;
      break;
    case 'd':
      dflag = 1;
      dvalue = optarg;
      break;
    case 'l':
      lflag = 1;
      break;
    default:
      fprintf(stderr, "Usage: %s [-c task] [-l]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if(iflag) {
    WINDOW *window;
    int startx = 5, starty = 3, c, highlight;

    initscr();
    clear();
    raw();
    noecho();
    cbreak();
    curs_set(0);

    startx = (80 - WIDTH);
    starty = (24 - HEIGHT);
    window = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(window, TRUE);

    while(1) {
      mvwprintw(window, starty, startx, "TODOs");

      if((task_list_size = read_tasks(tasks, task_list))) {
        for(i = 0; i < task_list_size; i++) {
          if(highlight == i) {
            wattron(window, A_REVERSE);
            mvwprintw(window, starty + (i + 2), startx, "%s", tasks[i]);
            wattroff(window, A_REVERSE);
          } else {
            mvwprintw(window, starty + (i + 2), startx, "%s", tasks[i]);
          }
        }
      } else {
        mvwprintw(window, starty + 2, startx, "None!");
      }

      wrefresh(window);

      c = wgetch(window);
      switch(c) {
      case KEY_UP:
        if(highlight == 0) {
          highlight = task_list_size - 1;
        } else {
          --highlight;
        }
        break;
      case KEY_DOWN:
        if(highlight == task_list_size - 1) {
          highlight = 0;
        } else {
          ++highlight;
        }
        break;
      }
    }

    curs_set(1);
    clrtoeol();
    refresh();
    endwin();
  } else {
    if(dflag) {
      if(delete_task(dvalue, task_list) == 0) {
        fprintf(stderr, "Could not delete task!");
      }
    }

    if(cflag) {
      if(create_task(cvalue, task_list) == 0) {
        fprintf(stderr, "Could not create task!");
      }
    }

    if(lflag) {
      printf("TODOs\n\n");

      if((task_list_size = read_tasks(tasks, task_list))) {
        for(i = 0; i < task_list_size; i++) {
          printf("%s\n", tasks[i]);
        }
      } else {
        printf("None!\n");
      }
    }
  }

  return 0;
}
