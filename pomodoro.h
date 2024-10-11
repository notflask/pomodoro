#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define COUNTER_PATH "~/.pomodoro"
#define PID_PATH "/tmp/pomodoro.pid"
#define POMODORO_LENGTH 1500

static char *pomodoro_expand_path(const char *path) {
  if (path[0] == '~') {
    const char *home = getenv("HOME");
    assert(home != NULL && "'HOME' env variable is not defined");
    char *expanded_path = (char *)malloc(strlen(home) + strlen(path) + 1);
    assert(expanded_path != NULL && "failed to mallocate expanded_path");
    strcpy(expanded_path, home);
    strcat(expanded_path, path + 1);
    return expanded_path;
  } else {
    return strdup(path);
  }
}

static int pomodoro_counter_get() {
  char *path = pomodoro_expand_path(COUNTER_PATH);
  FILE *file = fopen(path, "r");

  int counter = 0;

  if (file != NULL) {
    if (fscanf(file, "%d", &counter) != 1) {
      fprintf(stderr, "failed to read counter from file\n");
    }
    fclose(file);
  } else {
    fprintf(stderr, "file does not exist or cannot be opened\n");
  }

  free(path);
  return counter;
}

static void pomodoro_counter_set(int value) {
  char *path = pomodoro_expand_path(COUNTER_PATH);
  FILE *file = fopen(path, "w");

  if (file != NULL) {
    fprintf(file, "%d", value);
    fclose(file);
  } else {
    fprintf(stderr, "failed to open file for writing\n");
  }

  free(path);
}

static char *pomodoro_counter_format() {
  char *result = (char *)malloc(20 * sizeof(char));

  int seconds = pomodoro_counter_get();

  int hours = seconds / 3600;
  seconds %= 3600;
  int minutes = seconds / 60;
  seconds %= 60;

  if (hours > 0) {
    sprintf(result, "%dh %dm %ds", hours, minutes, seconds);
  } else if (minutes > 0) {
    sprintf(result, "%dm %ds", minutes, seconds);
  } else {
    sprintf(result, "%ds", seconds);
  }

  return result;
}

static void pomodoro_daemonize() {
  pid_t pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  if (setsid() < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  int fd = open("/dev/null", O_RDWR);
  dup(fd);
  dup(fd);
  dup(fd);
}

static void pomodoro_stop() {
  FILE *pid_file;
  int pid = 0;

  pid_file = fopen(PID_PATH, "r");

  if (pid_file) {
    fscanf(pid_file, "%d", &pid);
    fclose(pid_file);
  }

  if (pid > 0) {
    kill(pid, SIGTERM);
    printf("pomodoro was stopped at %s\nnow you can relax (like a lorax)\n",
           pomodoro_counter_format());
  } else {
    printf("pomodoro is not running u bum\n");
  }
}

static void pomodoro_loop() {
  int counter = pomodoro_counter_get();

  while (1) {
    sleep(1);

    if (counter < POMODORO_LENGTH) {
      counter++;
      pomodoro_counter_set(counter);
    } else {
      pomodoro_stop();
    }
  }
}

static void pomodoro_start() {
  printf("pomodoro is counting in background (started from %s)... start "
         "working!\n",
         pomodoro_counter_format());

  FILE *pid_file;
  pomodoro_daemonize();

  pid_file = fopen(PID_PATH, "w");
  if (pid_file) {
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);
  }

  pomodoro_loop();
}

static void pomodoro_show() {
  if (pomodoro_counter_get() == 0)
    return;

  printf("%s\n", pomodoro_counter_format());
}

static void pomodoro_reset() {
  pomodoro_stop();
  pomodoro_counter_set(0);
  printf("counter value has been reseted to zero\n");
}
