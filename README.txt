pomodoro(1)

==========================================

simple linux cli which counts time in seconds for pomodoro studying method.
usage: pomodoro <start|stop|show|reset>
subcommands description:
  pomodoro show:
    displays current time in format "1h 0m 0s"
  pomodoro start:
    starts the daemon and counts the time to POMODORO_LENGTH (25 mins = 1500 seconds)
  pomodoro stop:
    stops the daemon and counting
  pomodoro reset:
    resets the value of current pomodoro timer to zero

files being used by "pomodoro" process is "~/.pomodoro" for counting and "/tmp/pomodoro.pid" for saving the pid of daemon
