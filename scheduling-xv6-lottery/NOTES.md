

## Notes on processor:

- Assign slice of processor based to the # of tickets
- Each time slice, a randomized lottery determines the winner
- We assign tickets when the process is created; tickets are inherited (10 tickets)
- Most of the code will be in `proc.c` and `proc.h`
- See read() and sys_read() and argptr() to see how the pointer is passed to the kernel

## Sys calls

int settickets(int number)
- sets the number of tickets of the calling process; each process should get 1 ticket by default
- returns 0 if successful and -1 otherwise

int getpinfo(struct pstat *)
- returns info about all running processes
- we can use this to build a variant of the command ps
- Need to fill in the structure `pstat` in the kernel and pass the results to the user space

Utils
- Random number generator [x]
- Make a graph with results

## Files to update:

For new system call:
- syscall.c
- syscall.h
- sysproc.c
- user.h

For new user call:
- user.c
- user.h
