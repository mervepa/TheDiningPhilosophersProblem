

# The Dining Philosophers Problem
---
 This project involves implementing a solution to this problem using POSIX mutex locks and condition variables.

Implementation will require creating five philosophers, each identified by a number 0 . . 4. Each philosopher will run as a separate thread. Philosophers alternate between thinking and eating. To simulate both activities, have each thread sleep for a random period between one and three seconds.

Thread creation using Pthreads is covered in Section 4.4.1. When a philosopher wishes to eat, she invokes the function:

`pickup_forks(int philosopher_number)`

Implementation will require the use of POSIX condition variables.
