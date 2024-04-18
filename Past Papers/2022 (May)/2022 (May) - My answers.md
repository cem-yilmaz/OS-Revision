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
# Question 3