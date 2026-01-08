#pragma once // "Waaaah Waaah this isnt standard" yeah okay cry about it

struct processInfo {
  int state; // process state
  int ppid; // parent PID
  int sz; // size of process memory, in bytes
  int nfd; // number of open file descriptors in the process
  int nrswitch; // number context switches in
};