
Notes
- We need to build a shell called wish
- It will run in a loop called interactive mode
- It should create a process for each new command (exception of built-in commands)
- The shell should be able to support batch mode; that is receive an input file of commands
  - If you hit the EOF; call exit(0)
  - To parse the input into pieces we can call strsep()

Executing commands
- To execute commands we can call fork, exec and wait
  - For this project use execv; do not use system library.

Search paths
- The shell needs to find a path for the programs; this is called the search path of the shell. The path variables has a list of all dirs to search
  - The initial shell path will be '/bin'

Executing commands
- The shell won't implement these commands but call them
- To check if a file exists in a dir and is executable we can use access()
  - e.g., access("/bin/ls", X_OK)

Built-in commands
- Check whether the command is built-in or not. If so invoke it directly
- We want to implement exit, cd, and path

Redirection

For example, if a user types `ls -la /tmp > output`, nothing should be printed
on the screen. Instead, the standard output of the `ls` program should be
rerouted to the file `output`. In addition, the standard error output of
the program should be rerouted to the file `output` (the twist is that this
is a little different than standard redirection).

Parallel commands

We want to run commands in parallel when given an ampersand

```
wish> cmd1 & cmd2 args1 args2 & cmd3 args1
```

Use wait() to have them finish completing.
