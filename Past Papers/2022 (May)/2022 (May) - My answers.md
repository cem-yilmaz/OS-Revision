[[2022 (May) - Paper.pdf|Paper PDF]]

# Question 1
## a) 
Describe the differences between:
### i) \[2 marks]
an *operating system* and a *hypervisor* (*virtual machine monitor*), specifically distinguishing the type of abstractions offered by each
#### My Answer
An *Operating System* manages the hardware for user programs through the use of a **kernel**. This kernel abstracts *hardware access* to an interface the user programs must use to access hardware resources.
A *hypervisor* manages several *guest operating systems*, instead managing **access to the kernel** for each. The VMM abstracts *kernel access*; when a guest OS requests something from the kernel, the VMM *makes this request for the guest OS* to the kernel, providing the result back to the guest OS.
#### Mark Scheme
- *1 mark* for correctly stating type of abstractions both an OS and VMM provide
- *1 mark* for providing a clear difference between these two abstractions
 
### ii) \[2 marks]
*type 1* and *type 2* hypervisors
#### My Answer
*Type 1* hypervisors are *integrated into the OS themselves*, which allows them much more direct kernel access. *Type 2* hypervisors run *above the kernel in user space* (and thus need to request kernel resources, much less directly than *type 1*).
#### Mark Scheme
- *1 mark* for correctly describing *Type 1 hypervisors*
- *1 mark* for correctly describing *Type 2 hypervisors*
- **Marks may be dropped if differences aren't present in descriptions**
### iii) \[2 marks]
#### My Answer
*Trap-and-emulate* is a method in which the VMM catches "traps" in the *guest OS* caused by *requests for kernel access*. When an OS makes a *kernel access request*, this causes a trap which is picked up by the VMM. The VMM then takes the represented PCB and actually executes it on the CPU, requesting real kernel access for the host OS.

(*I can't explicitly remember what **binary translation** is, so this next section uses stuff from the **textbook**.*) *Binary Translation* is used in situations when *trap-and-emulate* would fail (such as in instructions that don't clearly distinguish between privileged and unprivileged instructions). The VMM (in *kernel mode*) checks to see if trap and emulate will fail (by looking ahead for *special instructions* that would escape this), and when they are to be executed it *translates* them to equivalent instructions that will properly use privileges.
#### Mark Scheme
- *1 mark* for describing *trap-and-emulate*
- *1 mark* for describing *binary translation*
- **Marks may be dropped if differences aren't present in descriptions**
## b)
UNIX-like operating systems use the `fork()` syscall to create a new process.
### i) \[2 marks]
How many processes are created by the following snippet of code? Explain or illustrate your answer, assume `fork()` returns without any error.
```C
#include <stdio.h>
#include <unistd.h>
#define LOOPS 5

int main()
{
	int i, ret;
	for (i = 0; i < LOOPS; i++)
		ret = fork()
	return 0;
}
```
#### My Answer
5\. The code runs for 5 loops, forking itself 5 times. As a result, *5 child processes* are created from the code snippet.
#### ChatGPT's answer
In UNIX-like operating systems, the `fork()` system call is used to create a new process. When `fork()` is called, it creates a new process by duplicating the calling process. The new process is called the child process, and the calling process is called the parent process.

In the code snippet provided, `fork()` is called inside a loop that runs 5 times (`LOOPS` is defined as 5). Each time through the loop, the current process (initially just the original process) calls `fork()`. The tricky part about `fork()` is that after the fork, there are two processes running the same code, and they will both continue from the point after the `fork()` call.

So here's how it works out, step by step:

1. The original process (let's call it P0) enters the loop and calls `fork()` the first time.
2. Now there are two processes: P0 (the parent) and P1 (the child).
3. Both P0 and P1 enter the next iteration of the loop and each calls `fork()` again.
4. Now there are four processes: P0, P1 (from the first fork), and P2 and P3 (new children from P0 and P1's second `fork()` calls).
5. Each of these four processes enters the next iteration of the loop and calls `fork()` again, resulting in 8 processes.
6. This doubling continues until the loop finishes. The number of processes will be 2^n where n is the number of iterations of the loop.

To calculate the exact number of new processes created, we can use the formula:
$$\text{Total number of processes created} = 2^{\text{LOOPS}} - 1$$ 
This is because the original process is not created by `fork()`, so we subtract 1.

Since `LOOPS` is 5:

$$\text{Total number of processes created} = 2^5 - 1 = 32 - 1 = 31$$

So, 31 new processes are created by the time the loop completes.
#### Mark Scheme
- *1 mark* for the correct number (31)
- *1 mark* for a good explanation
### ii) \[3 marks]
Now, let’s consider the next snippet of code. Write down what you expect to be printed to the console by the `printf()` function, and explain your reasoning in detail. Assume that all library calls (`fork()`, `wait()`, and `printf()`) return without any error.
```C
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define LOOPS 5

int main()
{
    int ret, i, j = -1;
    for (i = 0; i < LOOPS; i++) {
        ret = fork();
        if (ret > 0) { //parent
            wait(0);
            break;
        }
    }
    printf("ret is: %d %d\n", ret, j);
    j = i;
    return 0;
}
```
#### ChatGPT's answer
The code snippet provided introduces a loop that will iterate `LOOPS` times, which is defined as 5. Within the loop, the `fork()` system call is used to create new processes. After each fork, if the `fork()` call returns a positive value, the current process is the parent, and it waits for the child process to finish by calling `wait(0)` and then breaks out of the loop. The child process, having a `ret` value of 0, does not enter the `if` block and thus does not break out of the loop, allowing it to potentially perform further `fork()` calls depending on the loop iteration.

Here's what is expected to happen, step by step:

1. The program starts with the initial process entering the loop (`i=0`).
2. The `fork()` call is made. If we are in the parent process, `ret` will be the PID of the child, which is a positive number.
3. The parent process enters the `if` statement because `ret` is positive, calls `wait(0)` to wait for the child process to finish, then breaks out of the loop, ending its `fork()` calls.
4. The parent process then prints `ret` (the PID of the child) and `j`, which remains `-1` since `j = i;` is after the print statement and the parent process broke out of the loop before reaching it.
5. Meanwhile, the child process, which has `ret` equal to 0, does not enter the `if` block and continues to the next iteration of the loop (`i=1`). The child becomes a parent of its own child when it calls `fork()` again.
6. For each new child created, they continue looping and forking until they become parents themselves. When they do, they will break out of the loop and print their `ret` (which is now positive, the PID of their child) and `j` (which is still `-1`).

The output will consist of each process (after it becomes a parent) printing a line with `ret is:` followed by the PID of its child and `-1`. Each child that has not yet forked a child of its own (i.e., any process with `ret` still equal to 0) will not print anything and will not break out of the loop.

Given that `LOOPS` is 5, the original process will create a child, then wait and exit the loop. The child will then become a parent in the next iteration, and so on. You would end up with each process, except for the last child (which never enters the `if` and thus never becomes a parent), printing its `ret` and `j`. This pattern would result in an output that shows a series of process IDs and -1, reflecting the point at which each became a parent and broke out of the loop.

In terms of the number of print statements, there will be one print per parent process before they break out of the loop. Since the `fork()` is inside a loop, you'll get a cascading effect of forks, but due to the `break` statement, each process only prints once before exiting the loop. The final child process created will not print anything because it never enters the `if` statement (it will have a `ret` of 0 as it does not become a parent).




Let's walk through the code and predict the output. The code uses `fork()` inside a loop with a `wait(0)` and a `break` statement for the parent processes. The child processes do not wait and continue to loop until they fork their own child process.

Here's a step-by-step prediction of what will happen when the code is run, and what will be printed:

1. The original process (we'll call this P0) starts with `i=0` and `j=-1`. It calls `fork()` to create a child process (P1).
2. In P0 (the parent), `fork()` returns a positive value (the PID of P1), so it enters the `if` block, waits for P1 to finish, prints the value of `ret` (PID of P1) and `j` (which is still `-1`), and then breaks out of the loop. P0 exits the loop and the program with no further output.
   
   Output of P0: `ret is: <PID_of_P1> -1`

3. P1 (the child from the first fork) has `ret` as `0`, so it skips the `if` block and iterates the loop again with `i=1`. P1 calls `fork()` to create P2.
4. In P1 (now the parent of P2), `fork()` returns a positive value (the PID of P2), so it enters the `if` block, waits for P2 to finish, prints the value of `ret` (PID of P2) and `j` (which is still `-1`), and then breaks out of the loop. P1 exits the loop and the program with no further output.

   Output of P1: `ret is: <PID_of_P2> -1`

5. P2 (the child of P1) follows the same pattern: it becomes a parent in the next iteration of the loop and so on until P5.

   Output of P2: `ret is: <PID_of_P3> -1`
   
   ...

   Output of P5: `ret is: <PID_of_P6> -1`

6. Finally, P6 (the last child) will not print anything since it will not enter the `if` block, it will continue looping without creating any child (because it's the last iteration of the loop), so it will not print anything and `ret` will be `0`.

The exact PIDs cannot be predicted because they will be assigned by the operating system at runtime. However, the structure of the output will be consistent:

- The original process will print its child's PID and `-1`.
- Each subsequent child-turned-parent process will print its own child's PID and `-1`.
- The final child process will not print anything because it never becomes a parent.

Each parent process prints only once before the `break` statement exits the loop for that process. So, with `LOOPS` defined as 5, we would expect 5 print statements, each followed by the child's PID and `-1`, except for the very last child process which will not print anything.
#### My interpretation of that answer
In the code, the process forks and checks to see if it's a parent. If it is, it waits for it's child process to finish and then breaks out of the `for` loop and exits.

So, the initial process forks, then waits for it's child to finish. The child process does not get caught by the "parent trap" (as `ret` is initialised to 0), so will print it's own child ID (set by `ret = fork()`) alongside `j` (`-1`). `j` is then set to `i`, but we will see this does not actually matter for outputted statements.

Now that the child process is a *parent*, it *will get caught*, wait for it's own child process to finish, and then exit the program (*w/o outputting anything else*).

This process continues 5 times. This is because `i` keeps getting incremented, and eventually will result in 5 (making the `for` loop condition `i < LOOPS` *false*), skipping the loop contents (stopping *further* `fork`s), and resulting in total stop.

As a result, the output will look like:
```
ret is: cPID1 -1
ret is: cPID2 -1
ret is: cPID3 -1
ret is: cPID4 -1
ret is: cPID5 -1
```
(where `child PID`s are determined at runtime).
#### Mark Scheme
- *1 mark* for correct printed output
- *2 marks* for explanation
	- 1 for explaining the `wait(0)` thing
	- 1 for explaining why it terminates after 5?
## c)
In general, multi threading improves overall performance and efficiency of programs in execution
### i) \[2 marks]
Provide two examples where using multi threading **improves** the performance of a single-threaded program. Explain why each example can improve the performance.
#### My Answer
- A city simulation game, for example, has to factor in multiple *actors* (buildings, cars, people, electricity, etc) when iterating the simulation by a time unit (a "tick"). If this is multi-threaded, then it can scale to larger computers with more processing cores. If it were single threaded, then the speed of the core would be the major bottleneck, even with more processing cores (these would just sit rather idle).
- Imagine a major program, comprised of many processes, that also has a user interface. If this UI were multi-threaded, we could take use of shared memory pools while still maintaining the asynchronous nature of threads. This would improve performance (responsiveness) for the user, and also allows the other processes a larger share of the memory pool. If this was single threaded, we'd need more (duplicated) memory or have to share memory (which comes with a performance overhead).
#### Mark Scheme
- *1 mark **each** for* a good example, with a proper explanation on why multi-threading would be more performant than single threading.
### ii) \[2 marks]
Provide two examples where using multi threading **fails** to improve the performance of a single-threaded program. Explain why each example **fails** to improve the performance.
#### My Answer
- Imagine a program which prints Fibonacci numbers. Because this loops, but also requires previous answers, the flow of execution is strictly sequential. If threading was used to anticipate these instructions, it would produce inaccurate results. Therefore, no amount of threading would actually improve the performance of the program
- An I/O heavy program may have multiple threads that can call a signal. The program will likely have to incorporate several thread-specific checks to see which threads to interrupt and which to keep going. These checks will likely degrade the performance of the application compared to if it was single threaded, where I/O interrupting would be a simple and optimised process
#### Mark Scheme
- *1 mark **each** for* a good example, with a proper explanation on why multi-threading would fail to be more performant than single threading.
### iii) \[3 marks]
If an application contains 25% of code that is *inherently serial*, what will the maximum speed-up be if ran on a multi-core system with four processors? Explain your reasoning.
#### My Answer
Amdahl's Law states the speedup is bounded by a number of processors $N$ and a system with a portion $S$ that must be performed serially. $$\text{speedup} \leq \frac{1}{S+\frac{(1-S)}{N}}$$
Therefore, $$\frac{1}{1/4 + \frac{3}{4}{4}} = \frac{1}{\frac{1}{4} + \frac{3}{16}}=\frac{1}{7/16}=\frac{16}{7}$$
The maximum speedup would be $\frac{16}{7}\times$, or $2\frac{2}{7}\times$  
#### Mark Scheme
- *1 mark* for remembering Amdahl's law
- *1 mark* for explaining why we use the law
- *1 mark* for correct speed-up upper bound
## d) 
When multiple processes access a limited number of resources it is important to identify if the system of processes may deadlock or not.
### i) \[2 marks]
In such context explain with your own words when a system is said to be in a safe state. Describe how to determine if a system is in a safe state. 
#### My Answer
A system is in a safe state when *it is **not** the case* that all conditions for a deadlock are active:
 - Mutual exclusion
 - Hold and wait
 - No preemption
 - Circular wait

We can use algorithms (such as the bankers algorithm) to detect this.
### ii) \[3 marks]
Is the following system of 4 processes with 2 resources deadlocked? Explain your reasoning.

Current **Allocation Matrix**

|        | *R1* | *R2* |
| ------ | ---- | ---- |
| **P1** | 1    | 3    |
| **P2** | 4    | 1    |
| **P3** | 1    | 2    |
| **P4** | 2    | 0    |

Current **Needed Matrix**

|        | *R1* | *R2* |
| ------ | ---- | ---- |
| **P1** | 1    | 2    |
| **P2** | 4    | 3    |
| **P3** | 1    | 7    |
| **P4** | 5    | 1    |

Current **Availability Vector**

|        | *R1* | *R2* |
| ------ | ---- | ---- |
| **P1** | 1    | 4    |
#### ChatGPT's answer (rephrased by me)
If we can prove there is a sequence of processes that can be completed in that order with current allocation, the system is not deadlocked.

Looking at the **Needed Matrix**, P1 needs more of 1 more of R1 and 2 more of R2. We can give it this straight away. (Availability R1:0, R2:2).
However, now we cannot give any other resources right away since they all require R1.

But, if we let R1 finish, then we now have Availability (R1:3, R2:7). This now leaves the request from P2 the only one we can fill (sine all others require more than 3 from R1). The availability vector now looks like Availability R1:4, R2:9

P4 can now be met, taking the 3 from R1 (nothing needed to take from R2 just R1). We now are left with availability well within the reach for P2.

Proving that there is a path for non-deadlocked execution proves the system is *not deadlocked*.

# Question 2
## a)
About blocking and non-blocking I/O, and DMA
### i) \[2 marks]
Describe the difference between blocking and non-blocking I/O operations. Provide an example use case for each of them.
#### My Answer
Most I/O operations are *blocking* - the system will delay execution until the device (which due to it's physical nature, the completion time of which cannot consistently be predicted) has finished it's job. *Non-blocking* I/O is when the system *resumes normal execution **while waiting** for I/O*.

- *Blocking* I/O would be best used in scenarios such as secondary storage data transfer, where the rest of the program heavily depends on this fetched data and would not operate correctly in it's absence if resumed before fetching
- *Non-blocking* I/O would be best used to read mouse movements, as the rest of the program does not heavily depend on it and would result in a greater user experience if resumed while waiting for I/O (e.g. a video player may use the mouse for interface actions, but for most of it's runtime, the user will not be using any I/O devices).
### ii) \[2 marks]
Describe how Programmed I/O works and state it's disadvantages. Under what conditions is polling a sensible approach?
#### My Answer
Programmed I/O is the process of, during data I/O transfer, issuing individual I/O calls for each byte of data to be transferred. This can be wasteful for scenarios such as mass data transfer wherein the I/O overhead severely lengthens the completion time (compared to other I/O methods such as DMA, which only have to worry about the much less impactful overhead of memory transfer).

Polling is a sensible approach when many I/O devices are connected to the computer and may end their operation at any time (*read: unpredictable*). Polling, while expensive, means the CPU can immediately react to these devices in a safe and controlled manner.
### iii) \[2 marks]
What advantages does direct memory access (DMA) provide? Describe its operation as seen by a device driver in the operating system.
#### My Answer
DMA provides an advantage when mass amounts of data are to be transferred during I/O as it reduces the amount of I/O operations (supplementing with (less expensive) memory operations). The OS writes the data to be transferred to a DMA buffer, and then the device driver would simply read a single request to access DMA (as opposed to millions of individual byte read requests). The device driver would then handle the memory transfer operations, but in terms of I/O, the operation is handled as a single I/O call.
## b)
Consider the following set of processes, with their corresponding execution time (in arbitrary time units) and priorities (the *lower* the number, the *higher* the priority - so *1* is the highest *priority* here)@

| *Process* | *Execution Time* | *Priority* |
| --------- | ---------------- | ---------- |
| $P1$      | 2                | 2          |
| $P2$      | 4                | 4          |
| $P3$      | 1                | 2          |
| $P4$      | 7                | 1          |
| $P5$      | 3                | 3          |
The processes are assumed to have arrived in the order $P1, P2, P3, P4, P5$ all at time $0$, but $P5$ that %%grammatical error in past paper!!! lol!!%% arrives at time $6$.
### i) \[5 marks]
Illustrate the execution of these processes using the following algorithms:
- *First-come-first-served* (FCFS)
- *Non-preemptive priority*
- *Non-preemptive shortest job first* (SJF)
- *Preemptive shortest job first* (SJF)
- *Round-robin* (with time quantum of 2 units)
#### My Answer
##### First come first served

| *Time* | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| ------ | ---- | ---- | ---- | ---- | ---- |
| 0      | X    |      |      |      |      |
| 1      | X    |      |      |      |      |
| 2      |      | X    |      |      |      |
| 3      |      | X    |      |      |      |
| 4      |      | X    |      |      |      |
| 5      |      | X    |      |      |      |
| 6      |      |      | X    |      |      |
| 7      |      |      |      | X    |      |
| 8      |      |      |      | X    |      |
| 9      |      |      |      | X    |      |
| 10     |      |      |      | X    |      |
| 11     |      |      |      | X    |      |
| 12     |      |      |      | X    |      |
| 13     |      |      |      | X    |      |
| 14     |      |      |      |      | X    |
| 15     |      |      |      |      | X    |
| 16     |      |      |      |      | X    |
##### Non-preemptive priority
*I'm just going to assume this means FCFS with priority, since FCFS is a non-preemptive algorithm.*

| *Time* | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| ------ | ---- | ---- | ---- | ---- | ---- |
| 0      |      |      |      | X    |      |
| 1      |      |      |      | X    |      |
| 2      |      |      |      | X    |      |
| 3      |      |      |      | X    |      |
| 4      |      |      |      | X    |      |
| 5      |      |      |      | X    |      |
| 6      |      |      |      | X    |      |
| 7      | X    |      |      |      |      |
| 8      | X    |      |      |      |      |
| 9      |      |      | X    |      |      |
| 10     |      |      |      |      | X    |
| 11     |      |      |      |      | X    |
| 12     |      |      |      |      | X    |
| 13     |      | X    |      |      |      |
| 14     |      | X    |      |      |      |
| 15     |      | X    |      |      |      |
| 16     |      | X    |      |      |      |
##### Non-preemptive shortest job first
*I'm going to assume the major difference between this one and [[#Preemptive shortest job first]] is that P5 gets scheduled last in this one because it has to join the queue that the CPU has already decided upon for $P1$, $P2$, $P3$ and $P4$.*

| *Time* | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| ------ | ---- | ---- | ---- | ---- | ---- |
| 0      |      |      | X    |      |      |
| 1      | X    |      |      |      |      |
| 2      | X    |      |      |      |      |
| 3      |      | X    |      |      |      |
| 4      |      | X    |      |      |      |
| 5      |      | X    |      |      |      |
| 6      |      | X    |      |      |      |
| 7      |      |      |      | X    |      |
| 8      |      |      |      | X    |      |
| 9      |      |      |      | X    |      |
| 10     |      |      |      | X    |      |
| 11     |      |      |      | X    |      |
| 12     |      |      |      | X    |      |
| 13     |      |      |      | X    |      |
| 14     |      |      |      |      | X    |
| 15     |      |      |      |      | X    |
| 16     |      |      |      |      | X    |
##### Preemptive shortest job first

| *Time* | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| ------ | ---- | ---- | ---- | ---- | ---- |
| 0      |      |      | X    |      |      |
| 1      | X    |      |      |      |      |
| 2      | X    |      |      |      |      |
| 3      |      | X    |      |      |      |
| 4      |      | X    |      |      |      |
| 5      |      | X    |      |      |      |
| 6      |      | X    |      |      |      |
| 7      |      |      |      |      | X    |
| 8      |      |      |      |      | X    |
| 9      |      |      |      |      | X    |
| 10     |      |      |      | X    |      |
| 11     |      |      |      | X    |      |
| 12     |      |      |      | X    |      |
| 13     |      |      |      | X    |      |
| 14     |      |      |      | X    |      |
| 15     |      |      |      | X    |      |
| 16     |      |      |      | X    |      |

##### Round Robin
*The queue for Round Robin is FIFO*

| *Time* | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| ------ | ---- | ---- | ---- | ---- | ---- |
| 0      | X    |      |      |      |      |
| 1      | X    |      |      |      |      |
| 2      |      | X    |      |      |      |
| 3      |      | X    |      |      |      |
| 4      |      |      | X    |      |      |
| 5      |      |      |      | X    |      |
| 6      |      |      |      | X    |      |
| 7      |      | X    |      |      |      |
| 8      |      | X    |      |      |      |
| 9      |      |      |      | X    |      |
| 10     |      |      |      | X    |      |
| 11     |      |      |      |      | X    |
| 12     |      |      |      |      | X    |
| 13     |      |      |      | X    |      |
| 14     |      |      |      | X    |      |
| 15     |      |      |      |      | X    |
| 16     |      |      |      | X    |      |

### ii) \[2 marks]
For each of the scheduling algorithms in 2(b)i, what is the *turnaround time* of each process?
#### My Answer
- **Everything but Round robin (?)**
	- P1 - 2
	- P2 - 3
	- P3 - 1
	- P4 - 7
	- P5 - 3
- **Round Robin**
	- P1 - 2
	- P2 - 7
	- P3 - 1
	- P4 - 12
	- P5 - 5
### iii) \[2 marks]
For each of the scheduling algorithms in 2(b)i, what is the *waiting time* of each process?
#### My Answer
*Assuming this refers to **first execution**, and doesn't include **intermediate** waiting time (relevant for Round Robin)*
- **First come first served**
	- P1 - 0
	- P2 - 2
	- P3 - 6
	- P4 - 7
	- P5 - 14
- **Non-preemptive priority**
	- P1 - 7
	- P2 - 13
	- P3 - 9
	- P4 - 0
	- P5 - 10
- **Non-preemptive shortest job first**
	- P1 - 1
	- P2 - 3
	- P3 - 0
	- P4 - 7
	- P5 - 14
- **Preemptive shortest job first**
	- P1 - 1
	- P2 - 3
	- P3 - 0
	- P4 - 10
	- P5 - 7
- **Round Robin**
	- P1 - 0
	- P2 - 2
	- P3 - 4
	- P4 - 5
	- P5 - 11
### iv) \[2 marks]
Which of the algorithms in 2(b)i results in the minimum waiting time (over all processes)?
#### My answer
Preemptive shortest job first
## c)
Two processes, $Pa$ and $Pb$, have the following sequential execution patterns:
- $Pa$: \[CPU 4ms; I/O 2ms; CPU 4ms; I/O 2ms; CPU 4ms]
- $Pb$: \[CPU 1ms; I/O 2ms; CPU 1ms; I/O 2ms; CPU 1ms; I/O 2ms; CPU 1ms]

I/O operations for the two processes *do not interfere* with each other and *are blocking*.
### i) \[2 marks]
If the processes are run consecutively one after another, what is the elapsed time for all to complete?
#### My Answer
26ms
### ii) \[2 marks]
Sketch the execution pattern under non-preemptive scheduling and determine the total elapsed time for completion. You may assume that processes are scheduled in the order in which they become ready to run and that in the event of a tie $Pa$ has priority over $Pb$. You may further assume that the scheduler and context switches take negligible time
#### My Answer
*Assuming here that these requests come after another. As in, for $Pa$ it takes 4ms before the I/O request is issued. Additionally, for $Pb$, assuming the CPU request comes in **but the 2ms I/O request does not** until the CPU one is finished. If this is wrong please lmk.*

| *Time* | $Pa$ | $Pb$ |
| ------ | :--: | :--: |
| 0      |  X   |      |
| 1      |  X   |      |
| 2      |  X   |      |
| 3      |  X   |      |
| 4      |      |  X   |
| 5      |  X   |      |
| 6      |  X   |      |
| 7      |      |  X   |
| 8      |      |  X   |
| 9      |  X   |      |
| 10     |  X   |      |
| 11     |  X   |      |
| 12     |  X   |      |
| 13     |      |  X   |
| 14     |  X   |      |
| 15     |  X   |      |
| 16     |      |  X   |
| 17     |      |  X   |
| 18     |  X   |      |
| 19     |  X   |      |
| 20     |  X   |      |
| 21     |  X   |      |
| 22     |      |  X   |
| 23     |      |  X   |
| 24     |      |  X   |
| 25     |      |  X   |
## d) \[4 marks]
Mechanical disks (HDDs), or simply disks, still play an important role in storage systems despite the introduction of SSDs. For the sake of this exercise consider a disk drive with 64 cylinders, numbered from $0$ to $63$. The request queue has the following composition: $$32 \: 11 \: 17 \: 24 \: 30 \: 35 \: 39 \: 8 \: 40 \: 51$$
If the current position is at $26$, and the previous request served was at 24, compute the total distance (*in cylinders*) that the disk arm would move for each of the following algorithms:
- FIFO
- SSTF
- SCAN
- C-SCAN
#### My Answer
- **FIFO - 129** 
	- (0)+26->32=6
	- +32->11=21+6=27
	- +11->17=6+27=33
	- +17->24=7+33=40
	- +24->30=6+40=46
	- +30->35=5+46=51
	- +35->39=4+51=55
	- +39->8=31+55=86
	- +8->40=32+86=118
	- +40->51=11+118=**129**
- **SSTF - 72**
	- (0)+26->24=2
	- +24->30=2+6=8
	- +30->32=8+2=10
	- +32->35=10+3=13
	- +35->39=13+4=17
	- +39->40=17+1=18
	- +40->51=18+11=29
	- +51->17=29+34=63
	- +17->11=63+6=*69*
	- +11->8=69+3=**72**
- **SCAN - 68**
	- *all the prev request thing means is that we're going **up** and to the right*
	- (0)+26->30=4
	- +30->32=4+2=6
	- +32->35=6+3=9
	- +35->39=9+4=13
	- +39->40=13+1=14
	- +40->51=14+11=25
	- +51->24=25+27=52
	- +24->17=52+7=59
	- +17->11=59+6=65
	- +11->8=65+3=**68**
- **C-SCAN - 84**
	-  (0)+26->30=4
	- +30->32=4+2=6
	- +32->35=6+3=9
	- +35->39=9+4=13
	- +39->40=13+1=14
	- +40->51=14+11=25
	- +51->8=25+43=68
	- +8->11=68+3=71
	- +11->17=71+6=77
	- +17->24=77+7=**84**
# Question 3
## a)
Consider an operating system that uses hardware paging to provide virtual memory to applications.
### i) \[2 marks]
Explain how the hardware and operating system support for paging combine to prevent one process from accessing another's memory
#### My Answer
The physical memory is split into *frames* by the hardware - hardware representations of how much memory is **actually** available. The OS allocates *pages* from these frames, giving each application it's own logical address space starting at 0.

These combine to prevent cross-program accessing of memory as the OS manages memory calls to the pages, which get allocated to hardware-separated frames.
### ii) \[2 marks]
Explain how space and time overheads arise from use of paging.
#### My Answer
- **Time overheads** - Paging comes with an associated time cost to page, especially if a new page needs to be created. If a program accessing memory constantly needs to create new pages, it can spend more CPU time paging than actually executing (*Thrashing*)
- **Space overheads** - Pages may be allocated for bits of memory seldom/only-once accessed, and forgotten to be deallocated. These will remain in memory during execution, hogging space uselessly.
### iii) \[3 marks]
Explain how the Translation Lookaside Buffer (TLB) and hierarchical page tables help solving time and space overheads, and in what circumstances.
#### My Answer
In circumstances where we are constantly accessing the same bits of memory, the TLB acts as a cache for recently-accessed memory. This solves the time overhead of having to translate messages every time - we translate once, and as long as it stays in the TLB, we just have to lookup the translation (which is much faster than computing).

<s>Not a clue on the space benefits mind you. Will edit if found.</s>
Hierarchical page tables allow for the paging address to refer to the table and the offset within the table. This means, instead of taking on the space overheads associated with ridiculously large addresses to accommodate for the bottom of the table entries, we can reduce the size of a logical address massively by referring to table and offset within table.
## b)
Suppose an OS implements virtual memory paging. A virtual address is 32 bits long and a page comprises 1KB ($2^{10}$ bytes). A hierarchical two-level table is used. The first-level (outer) and second level (inner) page tables have $4096 \:(2^{12})$ and $1024\:(2^{10})$ entries respectively.

If a process uses the first 4MB ($2^{22}$) of its virtual address space, how much space would be occupied by the page tables for that process, when:
### i) \[2 marks]
Each page table entry occupies 4 bytes?
#### ChatGPT's answer, rephrased by me
- For a $2^{22}$ address space, we'd need $\frac{2^{22}}{2^10}=2^{12}$ pages.
- Each inner page table can reference $2^{10}$ pages, so we'd need $\frac{2^{12}}{2^{10}}=2^{2}$ inner page tables
- The size of the inner page tables is $2^{2}\text{ (num of page tables)}\times2^{10}\text{ ( number of pages in each page table)}\times 4 \:(2^{2}) (\text{ size of a page table entry})$
	- or $2^{14}$ for short
- Since each outer page table has $2^{12}$ bytes, and each entry takes up $4 \: (2^2)$ bytes, the space taken up by the outer page table is $2^{12}\times2^{2}=2^{14}$
- Therefore, *overall space occupied by the page tables for that process* = $2^{14}+2^{14}=2^{14}\times2^{1}=2^{15}$
### ii) \[2 marks]
Each page table entry occupies 8 bytes?
#### My answer
This changes the size of the both page tables to $2^{15}$, making the overall size $2^{16}$
## b) (again)
Now suppose that instead of a hierarchical two-level page table, a single-level page table is used, and again a process uses the first 4MB ($2^{22}$) of its 32bits virtual address space. How much space would be occupied by the page tables for that process, when:
### iii) \[2 marks]
Each page table entry occupies 4 bytes?
#### My Answer
- We have a page table of $2^{22}$ entries; with each entry occupying $4 \: (2^{2})$ bytes
- The size of this page table is $2^{22}\times2^{2} = 2^{24}$ bytes
### iv)
Each page table entry occupies 8 bytes?
#### My Answer
- This doubles the size of the page table, making it $2^{25}$ bytes instead
## c)
Consider a hypothetical machine with four pages of physical memory and eight pages of virtual memory (labelled `A`-`H`). given the following page reference requests: $$F \: E \: D \: C \: B \: A \: D \: B \: H \: B \: A \: A \: G \: A \: B \: G \: D \: F \: F \: E$$
For the FIFO, LRU, and second chance page replacement policies
- i) Show the sequence of page swaps, and \[*4 marks*]
- ii) Compute the number of page faults \[*2 marks*]
### My Answer
#### FIFO

| *Request* | *Action taken*                                          | *Pages in memory \[max 4]* |
| --------- | ------------------------------------------------------- | :------------------------: |
| X         | X                                                       |             X              |
| F         | Page not in memory, *swapping in F*                     |             F              |
| E         | Page not in memory, *swapping in E*                     |            F, E            |
| D         | Page not in memory, *swapping in D*                     |          F, E, D           |
| C         | Page not in memory, *swapping in C* \[Memory full]      |         F, E, D, C         |
| B         | Page not in memory, *memory full, swapping out F for B* |         E, D, C, B         |
| A         | Page not in memory, *memory full, swapping out E for A* |         D, C, B, A         |
| D         | Page in memory                                          |         D, C, B, A         |
| B         | Page in memory                                          |         D, C, B, A         |
| H         | Page not in memory, *memory full, swapping out D for H* |         C, B, A, H         |
| B         | Page in memory                                          |         C, B, A, H         |
| A         | Page in memory                                          |         C, B, A, H         |
| A         | Page in memory                                          |         C, B, A, H         |
| G         | Page not in memory, *memory full, swapping out G for C* |         B, A, H, G         |
| A         | Page in memory                                          |         B, A, H, G         |
| B         | Page in memory                                          |         B, A, H, G         |
| G         | Page in memory                                          |         B, A, H, G         |
| D         | Page not in memory, *memory full, swapping out B for D* |         A, H, G, D         |
| F         | Page not in memory, *memory full, swapping out A for F* |         H, G, D, F         |
| F         | Page in memory                                          |         H, G, D, F         |
| E         | Page not in memory, *memory full, swapping out E for H* |         G, D, F, E         |
*Page Faults:* 11
#### LRU
| *Request* | *Action taken*                                                 | *Page last accessed* | *Pages in memory \[max 4]* |
| --------- | -------------------------------------------------------------- | -------------------- | :------------------------: |
| X         | X                                                              | X                    |             X              |
| F         | Page not in memory, *swapping in F*                            | F                    |             F              |
| E         | Page not in memory, *swapping in E*                            | E                    |            F, E            |
| D         | Page not in memory, *swapping in D*                            | D                    |          F, E, D           |
| C         | Page not in memory, *swapping in C* \[Memory full]             | C                    |         F, E, D, C         |
| B         | Page not in memory, *memory full, LRU: F, swapping in B for F* | B                    |         E, D, C, B         |
| A         | Page not in memory, *memory full, LRU: E, swapping in A for E* | A                    |         D, C, B, A         |
| D         | Page in memory                                                 | D                    |         D, C, B, A         |
| B         | Page in memory                                                 | B                    |         D, C, B, A         |
| H         | Page not in memory, *memory full, LRU: C, swapping in H for C* | H                    |         D, B, A, H         |
| B         | Page in memory                                                 | B                    |         D, B, A, H         |
| A         | Page in memory                                                 | A                    |         D, B, A, H         |
| A         | Page in memory                                                 | A                    |         D, B, A, H         |
| G         | Page not in memory, *memory full, LRU: D, swapping in G for D* | G                    |         B, A, H, G         |
| A         | Page in memory                                                 | A                    |         B, A, H, G         |
| B         | Page in memory                                                 | B                    |         B, A, H, G         |
| G         | Page in memory                                                 | G                    |         B, A, H, G         |
| D         | Page not in memory, *memory full, LRU: H, swapping in D for H* | D                    |         B, A, G, D         |
| F         | Page not in memory, *memory full, LRU: B, swapping F for B*    | F                    |         A, G, D, F         |
| F         | Page in memory                                                 | F                    |         A, G, D, F         |
| E         | Page not in memory, *memory full, LRU: A, swapping E for A*    | E                    |         G, D, F, E         |
*Page Faults*: 11
#### Second chance page replacement
*The book describes the reference digit as an **8 bit byte**, so I'm going to use this. The slides mention a single (1) bit, but this would mean a second chance couldn't happen over more than one request. I'm adopting the idea of the circular list to keep sweeping over the list (translation: if all bits have a reference, they're all cleared and then FIFO just happens as usual).*

*also im using decimals to represent the reference instead of binary cause its easier*

| *Request* | *Action taken*                                                                                    | *Pages in memory (w/ reference byte) \[max 4]* |
| --------- | ------------------------------------------------------------------------------------------------- | ---------------------------------------------- |
| X         | X                                                                                                 | X                                              |
| F         | Page not in memory, *swapping in F*                                                               | F {8}                                          |
| E         | Page not in memory, *swapping in E*                                                               | F {7}, E {8}                                   |
| D         | Page not in memory, *swapping in D*                                                               | F {6}, E {7}, D {8}                            |
| C         | Page not in memory, *swapping in C* \[Memory full]                                                | F {5}, E {6}, D {7}, C {8}                     |
| B         | Page not in memory, *memory full, all pages referenced so all bits set to 0, swapping in B for F* | E {0}, D {0}, C {0}, B {8}                     |
| A         | Page not in memory, *memory full, E unreferenced, swapping A for E*                               | D {0}, C {0}, B {7}, A {8}                     |
| D         | Page in memory                                                                                    | D {8}, C {0}, B {6}, A{8}                      |
| B         | Page in memory                                                                                    | D {7}, C {0}, B {8}, A {7}                     |
| H         | Page not in memory, *memory full, C unreferenced, swapping H for C*                               | D {6}, B {7}, A {6}, H {8}                     |
| B         | Page in memory                                                                                    | D {5}, B {8}, A {5}, H {7}                     |
| A         | Page in memory                                                                                    | D {4}, B {7}, A {8}, H {6}                     |
| A         | Page in memory                                                                                    | D {3}, B {6}, A {8}, H {5}                     |
| G         | Page not in memory, *memory full, all pages referenced so all bits set to 0, swapping in G for D* | B {0}, A {0}, H {0}, G {8}                     |
| A         | Page in memory                                                                                    | B {0}, A {8}, H {0}, G {7}                     |
| B         | Page in memory                                                                                    | B {8}, A {7}, H {0}, G {6}                     |
| G         | Page in memory                                                                                    | B {7}, A {6}, H {0}, G {5}                     |
| D         | Page not in memory, *memory full, H unreferenced, swapping D for H*                               | B {6}, A {5}, G {4}, D {8}                     |
| F         | Page not in memory, *memory full, all pages referenced so all bits set to 0, swapping in F for B* | A {0}, G {0}, D {0}, F {8}                     |
| F         | Page in memory                                                                                    | A {0}, G {0}, D {0}, F {8}                     |
| E         | Page not in memory, *memory full, A unreferenced, swapping E for A*                               | G {0}, D {0}, F {7}, E {8}                     |
*Page Faults*: 11
## d)
Suppose you have a file system where the block size if $2$KB, a disk address is 4 bytes, and an `i-node` structure contains the disk addresses of: (a) 12 direct blocks, (b) $2\times$ indirect blocks, and (c) $2\times$ double indirect blocks. Moreover, the rest of the i-node can be used to store the very first bytes of the file - hence, if the file is small, it can be fully contained in the `i-node`.
 
 > *In answering the following questions, you do not need to simplify arithmetic expressions, but you should show the maths and explain your calculations.*
 
### i) \[2 marks]
What is the largest file that can be represented by an `i-node`?
#### ChatGPT's answer, rephrased by me
- The 12 direct blocks point to $2KB$ files, so that's $12 \times 2KB$ of data that can be stored in those
- The 2 indirect blocks can point to the addresses of$\frac{2KB}{\text{4 bytes per address}} = \frac{2048}{4} = 512$ blocks of $2KB$ each, leading to $2KB \times 512$ of data that can be stored in those
- The 2 double indirect blocks can point to the addresses of $512$ indirect blocks, allowing $2KB\times512^{2}$ of data that can be stored in those
- We then sum all of these up; the total data (and as such the largest file if we use all of it on this one) that can be stored by this system is $$2KB \times (12 + 512 + 512^{2})$$
### ii) \[2 marks]
Suppose you want to create a new file of the largest file size. How many free blocks on the disk would there need to be, in order to create that file (including the `i-node` related blocks and data blocks)?
#### ChatGPT's answer, rephrased by me
- Piggybacking off previous answer
	- 12 blocks for direct blocks
	- 2 indirect blocks, plus the $512$ direct blocks they point to
	- 2 double indirect blocks, plus the $512^{2}$ direct blocks they point to
- So in total, $12 + (2 + 512) + (2 + 512^{2}) = $528 + 512^2$ blocks
### iii) \[3 marks]
How many disk blocks must be read to access byte 16 of a file? What about the byte at the 4MB ($2^{22}$) mark?
#### ChatGPT's answer, rephrased by me
- *For byte 16*, this would be stored within the first (of the 12) direct blocks, so just *one disk block*
- *For byte $4$MB*, we can't store this on the direct blocks ($4MB > 12 \times 2KB$). We also can't store this on the indirect blocks ($2\times512\times2KB = 2MB < 4MB$)
- So we'd have to store this on the double indirect blocks, leading to *3 disk blocks to be read* (double indirect -> indirect -> direct block)