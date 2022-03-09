# smallsh
#### *Unix shell functionality, written in C*

## How to run
1. On the command line, enter the following: `make`

   OR, to set additional options, manually compile: `gcc --std=c99 -g -Wall -o smallsh main.c command.c shell.c`

2. To run the program, enter: `./smallsh`

## How to use
- Run commands as you would on a traditional CLI: `command [arg1 arg2 ...] [< input_file] [> output_file] [&]` where `&` is used to run processes in the background
- Supports the use of `Ctrl+C` to send SIGINT signal to end all foreground processes
- Supports the use of `Ctrl+Z` to run all processes in foreground-only mode 
- Uses expansion variable `$$` which expands into the process ID of the shell itself.

## Example
```
$ smallsh
: ls
junk   smallsh    smallsh.c
: ls > junk
: status
exit value 0
: cat junk
junk
smallsh
smallsh.c
: wc < junk > junk2
: wc < junk
       3       3      23
: test -f badfile
: status
exit value 1
: wc < badfile
cannot open badfile for input
: status
exit value 1
: badfile
badfile: no such file or directory
: sleep 5
^Cterminated by signal 2
: status &
terminated by signal 2
: sleep 15 &
background pid is 4923
: ps
PID TTY          TIME CMD
4923 pts/0    00:00:00 sleep
4564 pts/0    00:00:03 bash
4867 pts/0    00:01:32 smallsh
4927 pts/0    00:00:00 ps
:
: # that was a blank command line, this is a comment line
:
background pid 4923 is done: exit value 0
: # the background sleep finally finished
: sleep 30 &
background pid is 4941
: kill -15 4941
background pid 4941 is done: terminated by signal 15
: pwd
/nfs/stak/faculty/b/brewsteb/CS344/prog3
: cd
: pwd
/nfs/stak/faculty/b/brewsteb
: cd CS344
: pwd
/nfs/stak/faculty/b/brewsteb/CS344
: echo 4867
4867
: echo $$
4867
: ^C
: ^Z
Entering foreground-only mode (& is now ignored)
: date
Mon Jan  2 11:24:33 PST 2017
: sleep 5 &
: date
Mon Jan  2 11:24:38 PST 2017
: ^Z
Exiting foreground-only mode
: date
Mon Jan  2 11:24:39 PST 2017
: sleep 5 &
background pid is 4963
: date
Mon Jan 2 11:24:39 PST 2017
: exit $
```
