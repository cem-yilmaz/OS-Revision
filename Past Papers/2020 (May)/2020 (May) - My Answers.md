# Question 1
## a)
Briefly describe the three classic OS architectures. List the key advantages and drawbacks for each, and mention at least one well-known operating system that implements it
### My Answer
- **Microkernel**
	- A microkernel puts only the purely necessary code inside of the kernel; other functions may be provided but will run in user space alongside other applications. Example - MiniOS (*Minix*)
		- *Benefits*
			- kernel is lightweight
			- choices of secondary processes allows for more competition
				- putting "core" kernel features in user space allows users to "choose" their e.g. network resolver
		- *Drawbacks*
			- these user space "kernel" processes will need to cross kernel boundaries often, which incurs crossing overhead
- **Monokernel**
	- A monokernel puts all functionality an OS needs (aside from like, GUI) inside of the kernel. Example - FreeBSD
		- *Benefits*
			- Kernel processes have an unparalleled degree of interlinking between processes and there's virtually no overhead - the kernel is low level.
		- *Drawbacks*
			- Hard to debug
			- heavy kernel
- **Modular Kernel**
	- A Modular Kernel allows *extensible modules* to be inserted to *add functionality*. Example - Linux
		- *Benefits*
			- Allows any developer to change the kernel the way they see fit, personally
		- *Drawbacks*
			- comes with *some* overhead
#### BetterInf aside
Hybrid kernels, which is basically what I was getting at with modular kernels. Windows NT is a hybrid kernel.
## b)
Difference between *OS* and *hypervisor (VMM)*
### My Answer
An Operating System handles hardware allocation of resources stemming from the requests of *processes* running on it. A VMM handles hardware allocation of resources stemming from the requests of *operating systems* being virtualised on it, which in turn are brought from processes running on those.
## c)
Why is paravirtualisation a more efficient form of hardware virtualisation?
### My Answer
Paravirtualisation means the OS knows it is being virtualised and therefore that it is talking to a VMM. This means it can alter it's instructions to cause less overhead for the VMM, and more efficiently use the hardware.
## d)
What's the output of this code?
![[code.png]]
#### My Answer
- Parent won't print anything as $x \neq 0$  and $y \neq 0$.
- First fork makes PID 4225
	- this enters the `if`
	- **prints** "x = 0 y = 2"
	- then **prints** "I am process: 4225"
- Then *assuming the parent runs first*, the parent's second fork makes PID 4226
	- enters `if`
	- **prints** "x = 1 y = 0"
	- then **prints** "I am process 4226"
- However, now *`x`'s fork* gets into the second fork, making PID 4227
	- enters `if`
	- **prints** "x = 0 y = 0"
	- then **prints** "I am process 4227"
## e)
Explain *kernel* threading (1-1), *user-level* threading (M-1), and N-M threading
### My Answer
- Kernel
	- The kernel supports its own thread scheduling. Threads of a process are assigned a thread in the kernel and similar threading scheduling algorithms are used
- User level
	- The kernel has no idea of threads. *Many* Process threads are mapped to *one* single PCB in the kernel, and the overall instructions from the threads are read as if the program was just written like that
- N:M
	- The kernel has threads, but a thread may act like M:1 - as in, multiple user threads are allocated for a kernel thread. This means the kernel uses threading, but still abstracts the user's threads
## f)
Identify a sequence of code lines that when executed may result in a deadlock
### Using BetterInformatics
Allow the producer function to continually run _without running consumer once_ such that we keep running down(full) until full == 0. When we go to run producer again, we acquire the down mutex. However, we cannot progress past down(full) as we cannot further decrement full past 0; we need to wait for the consumer function to increment full by executing line 21. However, Consumer cannot acquire the down mutex as producer holds it. Producer cannot get it back up as it is blocked at down(full) - we are deadlocked.

so we'd do $3\times$ 6->12, then do 6->8, then run 17->20. We then try to run 9, but we can't. We then go "okay, maybe release it" but trying to run 21 is blocked by 20.
# Question 2
## a)
Of I/O-bound, and CPU-bound programs, which is more likely to incur voluntary context switches (i.e. purposefully give back control of the CPU to the OS), and which is more likely to incur involuntary context switches (i.e. the OS forcibly interrupts the program’s execution)? Explain why this is the case.
### My Answer
I/O bound is most likely to volunteer up control of the CPU. If I/O is taking an exceptionally long time, the program will voluntarily give up the CPU until it receives an interrupt the I/O. CPU bound programs will really want a ton of computation at all time, so any context switches will happen *involuntarily* (kicking and screaming).
## c)
explain fifo rr and mlfq
### My Answer
- fifo - first process scheduled is first process computed
- rr - set a time quantum. alternate between the processes, computing for that quantum time until all done
- mlfq - *(BetterInf'ing it)* - RR with different queues depending on priority, each queue having a different quanta. Any process that exceeds it's quanta is moved down
## d)
Do FCFS, SJF, NPP, and RR (2)
### My Answer
#### FCFS

|     | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| --- | ---- | ---- | ---- | ---- | ---- |
| 0   | X    |      |      |      |      |
| 1   | X    |      |      |      |      |
| 2   | X    |      |      |      |      |
| 3   | X    |      |      |      |      |
| 4   | X    |      |      |      |      |
| 5   |      | X    |      |      |      |
| 6   |      | X    |      |      |      |
| 7   |      |      | X    |      |      |
| 8   |      |      |      | X    |      |
| 9   |      |      |      | X    |      |
| 10  |      |      |      | X    |      |
| 11  |      |      |      | X    |      |
| 12  |      |      |      | X    |      |
| 13  |      |      |      | X    |      |
| 14  |      |      |      | X    |      |
| 15  |      |      |      |      | X    |
| 16  |      |      |      |      | X    |
| 17  |      |      |      |      | X    |
| 18  |      |      |      |      | X    |
#### SJF
|     | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| --- | ---- | ---- | ---- | ---- | ---- |
| 0   |      |      | X    |      |      |
| 1   |      | X    |      |      |      |
| 2   |      | X    |      |      |      |
| 3   |      |      |      |      | X    |
| 4   |      |      |      |      | X    |
| 5   |      |      |      |      | X    |
| 6   |      |      |      |      | X    |
| 7   | X    |      |      |      |      |
| 8   | X    |      |      |      |      |
| 9   | X    |      |      |      |      |
| 10  | X    |      |      |      |      |
| 11  | X    |      |      |      |      |
| 12  |      |      |      | X    |      |
| 13  |      |      |      | X    |      |
| 14  |      |      |      | X    |      |
| 15  |      |      |      | X    |      |
| 16  |      |      |      | X    |      |
| 17  |      |      |      | X    |      |
| 18  |      |      |      | X    |      |
#### NPP
|     | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| --- | ---- | ---- | ---- | ---- | ---- |
| 0   | X    |      |      |      |      |
| 1   | X    |      |      |      |      |
| 2   | X    |      |      |      |      |
| 3   | X    |      |      |      |      |
| 4   | X    |      |      |      |      |
| 5   |      |      | X    |      |      |
| 6   |      |      |      | X    |      |
| 7   |      |      |      | X    |      |
| 8   |      |      |      | X    |      |
| 9   |      |      |      | X    |      |
| 10  |      |      |      | X    |      |
| 11  |      |      |      | X    |      |
| 12  |      |      |      | X    |      |
| 13  |      |      |      |      | X    |
| 14  |      |      |      |      | X    |
| 15  |      |      |      |      | X    |
| 16  |      |      |      |      | X    |
| 17  |      | X    |      |      |      |
| 18  |      | X    |      |      |      |
#### rr (2)

|     | $P1$ | $P2$ | $P3$ | $P4$ | $P5$ |
| --- | ---- | ---- | ---- | ---- | ---- |
| 0   | X    |      |      |      |      |
| 1   | X    |      |      |      |      |
| 2   |      | X    |      |      |      |
| 3   |      | X    |      |      |      |
| 4   |      |      | X    |      |      |
| 5   |      |      |      | X    |      |
| 6   |      |      |      | X    |      |
| 7   |      |      |      |      | X    |
| 8   |      |      |      |      | X    |
| 9   | X    |      |      |      |      |
| 10  | X    |      |      |      |      |
| 11  |      |      |      | X    |      |
| 12  |      |      |      | X    |      |
| 13  |      |      |      |      | X    |
| 14  |      |      |      |      | X    |
| 15  | X    |      |      |      |      |
| 16  |      |      |      | X    |      |
| 17  |      |      |      | X    |      |
| 18  |      |      |      | X    |      |
## e)
![[MLQF.png]]

|     | P1       | P2       | P3       | P4       | P5       | P6       |
| --- | -------- | -------- | -------- | -------- | -------- | -------- |
| 5   | X        |          |          |          |          |          |
| 10  | X        |          |          |          |          |          |
| 15  | X - DONE |          |          |          |          |          |
| 20  |          |          | X        |          |          |          |
| 25  |          |          | X        |          |          |          |
| 30  |          |          |          | X        |          |          |
| 35  |          |          |          | X        |          |          |
| 40  |          | X        |          |          |          |          |
| 45  |          |          |          |          | X - DONE |          |
| 50  |          |          | X        |          |          |          |
| 55  |          |          |          |          |          | X        |
| 60  |          |          |          |          |          | X        |
| 65  |          |          | X - DONE |          |          |          |
| 70  |          |          |          | X        |          |          |
| 75  |          |          |          | X - DONE |          |          |
| 80  |          | X        |          |          |          |          |
| 85  |          | X        |          |          |          |          |
| 90  |          |          |          |          |          | X        |
| 95  |          |          |          |          |          | X - DONE |
| 100 |          | X        |          |          |          |          |
| 105 |          | X - DONE |          |          |          |          |
![[THE RIGHT ANSWER.png]]
# Question 3
## a)
What is the difference between a *virtual page*, and a *page frame*?
### Answer
- Virtual pages are slices of virtual memory that aren't real. The OS will reference them to frames, slices that are real.