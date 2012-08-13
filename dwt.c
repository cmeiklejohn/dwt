#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

const char TASK_LIST_FILE[] = "tasks.txt";

FILE * open_task_list() {
  FILE *task_list;

  task_list = fopen(TASK_LIST_FILE, "a+");

  if(task_list == NULL) {
    fprintf(stderr, "Can't open task list: %s\n", TASK_LIST_FILE);
  }

  return task_list;
}

void close_task_list(FILE * task_list) {
  fclose(task_list);
}

int main(int argc, char *argv[]) {
  int option, cflag, lflag, iflag;
  char task[100];
  FILE *task_list;

  while((option = getopt(argc, argv, "ilc:")) != -1) {
    switch(option) {
    case 'i':
      iflag = 1;
      break;
    case 'c':
      cflag = 1;
      break;
    case 'l':
      lflag = 1;
      break;
    default:
      fprintf(stderr, "Usage: %s [-c task] [-l]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  task_list = open_task_list();

  if(iflag) {
    initscr();
    raw();
    noecho();

    printw("TODOs");
    refresh();

    getch();
    endwin();
  } else {
    if(cflag) {
      fputs(optarg, task_list);
      fputc('\n', task_list);
    }

    if(lflag) {
      while(fgets(task, sizeof(task), task_list) != NULL) {
        printf("%s", task);
      }
    }
  }

  close_task_list(task_list);

  return 0;
}
