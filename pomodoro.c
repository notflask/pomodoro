#include "pomodoro.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: pomodoro <start|stop|show|reset>\n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "start") == 0) {
    pomodoro_start();
  } else if (strcmp(argv[1], "stop") == 0) {
    pomodoro_stop();
  } else if (strcmp(argv[1], "show") == 0) {
    pomodoro_show();
  } else if (strcmp(argv[1], "reset") == 0) {
    pomodoro_reset();
  } else {
    printf("unknown command '%s'\n", argv[1]);
    exit(EXIT_FAILURE);
  }
}
