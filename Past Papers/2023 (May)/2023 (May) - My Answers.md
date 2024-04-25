[[2023 (May) - Paper.pdf|Paper PDF]]
# Question 1
## a)
Describe the differences between:
### i) \[2 marks]
*full-virtualisation* and *para-virtualisation*, specifically hint at what are the benefits and drawbacks of each approach;
#### Parroting off [BetterInformatics answer](https://files.betterinformatics.com/exams/0mrvlrt4.pdf#aje1yx3huw69w1xj)
Full virtualisation is when the operating system is *unaware* it is being virtualised. Para-virtualisation is when the operating system *is aware* it is being virtualised. Para-virtualisation allows the OS to communicate efficiently with the VMM (since it knows it doesn't have to communicate with hardware via a kernel), which provides better performance due to the lack of an overhead compared to full virtualisation. However, not all software is compatible with para-virtualised VMs, compared to full compatability (*if programmed with no faults, so this is kinda theoretical*) on full-virt VMs.
- BI also mentions cross-arch compat if PV enabled
### ii) \[2 marks]
*type 1* and *type 2* hypervisors;
#### My Answer
Type 1 hypervisors are integrated into the OS themselves, relying on little overhead-inducing intervention. Type 2 are applications that run *above* the OS, relying on heavier intervention due to their relatively low direct integration.

*Additionally, Type 2 usually run each seperate VM instance as **their own process**, not like Type 1 where the VMM is a big stonking heavy process*.
### iii) \[2 marks]
*machine* and *language* virtualisation
#### My Answer (bad)
<s>Is this binary translation and trap-and-emulate? If so answer from 2022</s>
#### [BetterInformatics answer](https://files.betterinformatics.com/exams/0mrvlrt4.pdf#fmhvhymvzof7io3y), rephrased in my own image
Machine virtualisation is when the full machine is virtualised on another machine. Software such as *VirtualBox* does this. Language virtualisation is when a programming language can be *virtualised* on another machine through use of a virtual machine; this is mentality behind the *Java Virtual Machine*.
## b)
`UNIX`-like operating systems use the `fork()` syscall to create a new process.
### i) 
How many processes are created by the following snippet of code (*including the main process*)?
```C
#include <stdio.h>
#include <unistd.h>

int main() {
	fork();
	fork();
	fork();
	fork();
	fork();
	return 0
}
```
#### My Answer
Line 4 - 1
Line 5 - 2 (forked once)
Line 6 - 4 (2 forked themselves)
Line 7 - 8 (4 forked themselves)
Line 8 - 16
Line 9 - 32

32
### ii)
Write down what should be expected to be printed from this snippet of code:
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int ret, i, j = -1;
	for (i = 0; i < 5; i++) {
		ret = fork();
		if (ret > 0) { //parent
			wait(0);
			printf('ret is: %d %d\n', ret, j);
			break;
		}
		j = i;
	}
	return 0;
}
```
#### My Answer
We'd expect to see 5 statements printed. Each would have a child process ID (determined by tons of factors outside the scope of the question so can't be predicted), this is what `ret` would be. We would also see `j` increase in value for each child, ranging inclusively from `-1` all the way to `3` (since we would use `0`).

We'd expect to see:
```
ret is: cPID1 3
ret is: cPID2 2
ret is: cPID3 1
ret is: cPID4 0
ret is: cPID5 1
```
## c)
In general, multi-threading improves the overall performance and efficiency of programs in execution
### i) \[2 marks]
Provide two examples where using multi-threading **improves** the performance of a single-threaded program. Explain why each example can improve the performance.
#### My Answer
1. A web server could service multiple requests from different users by servicing each of these requests in a separate thread. This would increase the performance as these requests may involve several "sub-goals"; single threading these means only one user at a time gets all of their goals, whereas multi-threading increases perceived performance by giving more users one of these sub-goals faster.
2. An image processing software could apply an effect (such as a mask) to different parts of the image in parallel by using threads
### ii) \[2 marks]
Provide two examples where using multi-threading **fails to improve** the performance of a single-threaded program. Explain why each example **fails** to improve the performance.
#### My Answer
1. An I/O heavy program may decide to utilise threading. However, if the I/O device needs to send a signal, then the safest method of handling this with multi-threading is by checking every single thread. This overhead will negate any benefit threading may have, and could possibly make the program slower if I/O signals happen a lot.
2. A program that counts the Fibonacci numbers would not benefit from threading as the instruction order is inherently sequential - it **requires** the previously computed number. We cannot use threading to compute these in parallel due to this nature. Even if different threads computed different numbers, execution cannot happen on multiple CPUs at the same time.
### iii) \[3 marks]
Explain or illustrate the main difference between *user-level-threading* and *kernel-level-threading*. In addition:
- A. Describe one case in which user-level threading **is more convenient** (or provides better performance) than kernel-level threading.
- B. Describe one case in which user-level threading **is less convenient** (or provides better performance) than kernel-level threading.
#### My Answer
User-level threading *appears* as individual processes to the kernel when it needs to schedule. The kernel cannot distinguish between threads of a single process and a collection of multiple processes.
#### Checking [BetterInf](https://files.betterinformatics.com/exams/0mrvlrt4.pdf#1rlsnrmdwa69fsil) and [[Operating Systems - Revision|notes]] for ups and downs
- User level threading provides better performance through this "process abstraction" as it does not use expensive `syscalls` and instead uses the (much more peformant) process calls
- User level threading is less convenient as, with kernel-level threading, the kernel can handle situations where threading may result in deadlocks due to its threading tools. If we ignore these and opt for user-level, we will have to implement such safety features ourselves (much less convenient)
## d)
When multiple processes access a limited number of resources it is important to identify if the system of processes may deadlock or not.
### i) \[2 marks]
In such context explain with your own words when a system is said to be in a safe state. Describe how we can tell if it is.
#### My Answer
A system is in a safe state if there is an order these processes can execute in such that there will always be enough resources for the next process to execute. We can determine this by finding an order.
#### Aside from [BetterInf](https://files.betterinformatics.com/exams/0mrvlrt4.pdf#o6ucrwt7nkw52wk5)
A system is said to be in a safe state given that there is no danger that said system can end up in a deadlock.

You can determine if a system is in a safe state if it does not fulfil all 4 criteria outlining the risk of a deadlock:

Mutual exclusion - Each resource is either currently being used by **exactly** one thread/process, or it is available.

Hold and wait - Processes/threads that are currently holding resources granted to it earlier can go on to request new resources.

Circular wait - There must be a circular list of two or more processes/threads, each waiting for a resource held by the next member in the chain (see the 5 dining philosophers problem).

No preemption - Resources previously given cannot be forcibly taken back and must be explicitly given back to the system by the thread/process holding them.

A system must fulfill all of these, or it cannot deadlock and is therefore safe. Fulfilling all 4 does not necessarily mean a system _is_ in deadlock, it merely implies that it can be, and is said therefore to be in an unsafe state.

### ii) \[3 marks]
Is the following system of four processes deadlocked?

Current **Allocation Matrix**:

|     | *R1* | *R2* |
| --- | ---- | ---- |
| P1  | 1    | 3    |
| P2  | 4    | 1    |
| P3  | 1    | 2    |
| P4  | 2    | 0    |
Current **Needed Matrix**:

|     | *R1* | *R2* |
| --- | ---- | ---- |
| P1  | 1    | 2    |
| P2  | 4    | 3    |
| P3  | 1    | 7    |
| P4  | 2    | 0    |

Current **Availability Vector**

| *R1* | *R2* |
| ---- | ---- |
| 2    | 4    |
#### My Answer
P1 can immediately go first, as it required {R1:1, R2:2} which is more than available. After this, availability looks like 

| *R1* | *R2* |
| ---- | ---- |
| 3    | 7    |
We can now run P3 (needs {R1:1, R2:7}). This then leaves us availability of

| *R1* | *R2* |
| ---- | ---- |
| 4    | 9    |
We can now run P2 (needs {R1:4, R2:4}). This leaves us with the availability of 

| *R1* | *R2* |
| ---- | ---- |
| 8    | 10   |
Now we can run P4 (needs {R1:5, R2:1}). We have ran all processes *without a deadlock*.
### iii) \[2 marks]
What if we assume the following **availability vector** instead?

| *R1* | *R2* |
| ---- | ---- |
| 1    | 4    |
#### My Answer
Like last time, we can only run P1 first, leaving us **AV**: 

| *R1* | *R2* |
| ---- | ---- |
| 2    | 7    |
We run P3, leaving us

| *R1* | *R2* |
| ---- | ---- |
| 3    | 9    |
Since both P2 and P4 require $>3\text{ of R1}$, we **are deadlocked** 
# Question 2
## a)
*Skipping **a)** for now because I've already done it and it's kinda bad revision.*
## b)
Consider the following set of processes (lower priority numbers are higher priority):

| Process | Execution Time | Arrival Time | Priority |
| ------- | :------------: | :----------: | :------: |
| P1      |       4        |      0       |    2     |
| P2      |       3        |      0       |    3     |
| P3      |       1        |      0       |    4     |
| P4      |       8        |      3       |    1     |
| P5      |       1        |      6       |    2     |
### i) \[5 marks]
Illustrate the execution of these processes using:
- FCFS
- Non-preemptive priority
- SJF (preemptive and non-preemptive)
- Round-Robin
#### My answer
##### FCFS

|     | *P1* | *P2* | *P3* | *P4* | *P5* |
| --- | :--: | :--: | :--: | :--: | :--: |
| 0   |  X   |      |      |      |      |
| 1   |  X   |      |      |      |      |
| 2   |  X   |      |      |      |      |
| 3   |  X   |      |      |      |      |
| 4   |      |  X   |      |      |      |
| 5   |      |  X   |      |      |      |
| 6   |      |  X   |      |      |      |
| 7   |      |      |  X   |      |      |
| 8   |      |      |      |  X   |      |
| 9   |      |      |      |  X   |      |
| 10  |      |      |      |  X   |      |
| 11  |      |      |      |  X   |      |
| 12  |      |      |      |  X   |      |
| 13  |      |      |      |  X   |      |
| 14  |      |      |      |  X   |      |
| 15  |      |      |      |  X   |      |
| 16  |      |      |      |      |  X   |
##### Non-preemptive priority
|     | *P1* | *P2* | *P3* | *P4* | *P5* |
| --- | :--: | :--: | :--: | :--: | :--: |
| 0   |      |      |      |  X   |      |
| 1   |      |      |      |  X   |      |
| 2   |      |      |      |  X   |      |
| 3   |      |      |      |  X   |      |
| 4   |      |      |      |  X   |      |
| 5   |      |      |      |  X   |      |
| 6   |      |      |      |  X   |      |
| 7   |      |      |      |  X   |      |
| 8   |  X   |      |      |      |      |
| 9   |  X   |      |      |      |      |
| 10  |  X   |      |      |      |      |
| 11  |  X   |      |      |      |      |
| 12  |      |      |      |      |  X   |
| 13  |      |  X   |      |      |      |
| 14  |      |  X   |      |      |      |
| 15  |      |  X   |      |      |      |
| 16  |      |      |  X   |      |      |
##### SJF (non-preemptive)
|     | *P1* | *P2* | *P3* | *P4* | *P5* |
| --- | :--: | :--: | :--: | :--: | :--: |
| 0   |      |      |  X   |      |      |
| 1   |      |  X   |      |      |      |
| 2   |      |  X   |      |      |      |
| 3   |      |  X   |      |      |      |
| 4   |  X   |      |      |      |      |
| 5   |  X   |      |      |      |      |
| 6   |  X   |      |      |      |      |
| 7   |  X   |      |      |      |      |
| 8   |      |      |      |      |  X   |
| 9   |      |      |      |  X   |      |
| 10  |      |      |      |  X   |      |
| 11  |      |      |      |  X   |      |
| 12  |      |      |      |  X   |      |
| 13  |      |      |      |  X   |      |
| 14  |      |      |      |  X   |      |
| 15  |      |      |      |  X   |      |
| 16  |      |      |      |  X   |      |
##### SJF (preemptive)
|     | *P1* | *P2* | *P3* | *P4* | *P5* |
| --- | :--: | :--: | :--: | :--: | :--: |
| 0   |      |      |  X   |      |      |
| 1   |      |  X   |      |      |      |
| 2   |      |  X   |      |      |      |
| 3   |      |  X   |      |      |      |
| 4   |  X   |      |      |      |      |
| 5   |  X   |      |      |      |      |
| 6   |      |      |      |      |  X   |
| 7   |  X   |      |      |      |      |
| 8   |  X   |      |      |      |      |
| 9   |      |      |      |  X   |      |
| 10  |      |      |      |  X   |      |
| 11  |      |      |      |  X   |      |
| 12  |      |      |      |  X   |      |
| 13  |      |      |      |  X   |      |
| 14  |      |      |      |  X   |      |
| 15  |      |      |      |  X   |      |
| 16  |      |      |      |  X   |      |
##### Round-robin (QUANTUM 4)

|     | *P1* | *P2* | *P3* | *P4* | *P5* |
| --- | :--: | :--: | :--: | :--: | :--: |
| 0   |  X   |      |      |      |      |
| 1   |  X   |      |      |      |      |
| 2   |  X   |      |      |      |      |
| 3   |  X   |      |      |      |      |
| 4   |      |  X   |      |      |      |
| 5   |      |  X   |      |      |      |
| 6   |      |  X   |      |      |      |
| 7   |      |      |  X   |      |      |
| 8   |      |      |      |  X   |      |
| 9   |      |      |      |  X   |      |
| 10  |      |      |      |  X   |      |
| 11  |      |      |      |  X   |      |
| 12  |      |      |      |      |  X   |
| 13  |      |      |      |  X   |      |
| 14  |      |      |      |  X   |      |
| 15  |      |      |      |  X   |      |
| 16  |      |      |      |  X   |      |
### ii)
What's the turnaround time for each of these processes?
#### BetterInf Answer
Turnaround time = end time - arrival time.

| Process | FCFS | SJF | SJF Preemptive | RR  | Non-preemptive Priority |
| :-----: | :--: | :-: | :------------: | :-: | :---------------------: |
|   P1    |  4   |  8  |       9        |  4  |            4            |
|   P2    |  7   |  4  |       4        |  7  |           16            |
|   P3    |  8   |  1  |       1        |  8  |           17            |
|   P4    |  13  | 14  |       14       | 14  |            9            |
|   P5    |  11  |  3  |       1        |  7  |            7            |
### iii)
What's the waiting time then?
#### BetterInf answer
Wait times:

| Process | FCFS | SJF | SJF Preemptive | RR | Non-preemptive Priority |
| :-----: | :--: | :-: | :------------: | :--: | :---------------: |
| P1 | 0 | 4 | 5 | 0 | 0 |
| P2 | 4 | 1 | 2 | 4 | 13 |
| P3 | 7 | 0 | 0 | 7 | 16 |
| P4 | 8 | 9 | 9 | 9 | 4 |
| P5 | 16 | 8 | 6 | 12 | 12 |


## c)
![[got bored of typing the questions lol.png]]
### i) \[2 marks]
If all processes are to run consecutively one after another, what is the elapsed time for all to complete?
#### My Answer
- Pa: 4 + 2 + 4 + 2 + 4 = 14
- Pb: 3 + 2 + 3 + 2 + 3 = 13
- Pc: 1 + 2 + 1 + 2 + 1 + 2 + 1 = 10
- Pb: 3 + 3 + 3 = 9
total = 46
### ii)
Sketch exec pattern under non-preemptive sched (prio: Pa > Pb > Pc > Pd)
#### Ross' answer
![[Ross' answer.png]]
## d)
You have the following request queue. Current pos was 15, prev was 13 (meaning we're going up)
![[d) request queue.png]]
Use FIFO, SSTF, SCAN, and C-SCAN and compute total distance for each
#### My Answer
- **FIFO - 229**
	- (0)+15->23=8
	- 8+23->1=8+22=30
	- 30+1->27=30+26=56
	- 56+27->14=56+13=*69*
	- *69*+14->35=69+21=90
	- 90+35->30=90+5=95
	- 95+30->39=95+9=104
	- 104+39->8=104+31=135
	- 135+8->51=135+43=178
	- 178+51->40=178+11=189
	- 189+40->31=189+9=198
	- 198+31->62=198+31=**229**
- **SSTF - 75**
	- (0)+15->14=1
	- 1+14->8=1+6=7
	- 7+8->1=7+7=14
	- 14+1->23=14+22=36
	- 36+23->27=36+4=40
	- 40+27->30=40+3=43
	- 43+30->31=43+1=44
	- 44+31->35=44+4=48
	- 48+35->39=48+4=52
	- 52+39->40=52+1=53
	- 53+40->51=53+11=64
	- 64+51->62=64+11=**75**
- **SCAN - 111**
	- (0)+15->23=8
	- 8+23->27=8+4=12
	- 12+27->30=12+3=15
	- 15+30->31=15+1=16
	- 16+31->35=16+4=20
	- 20+35->39=20+4=24
	- 24+39->40=24+1=25
	- 25+40->51=25+11=36
	- 36+51->62=36+11=47
	- 47+62->14=47+58=105
	- then add 6 -> **111**
- **C-SCAN** is 6 more, since it goes from 62->8 (6 more), and then 6 back up; **117**
# Question 3
## a)
Consider an operating system that uses hardware paging to provide virtual memory to applications
### i) \[2 marks]
How does the use of virtual memory improve system utilisation? Provide at least an example of that?
#### My Answer
Virtual Memory improves system utilisation of resources by providing a seemingly unlimited memory interface to applications and then smartly allocating this based on the *actual memory needed*. For example, if we strictly allocated memory physically, a program which is sometimes a resource hog would likely require this memory fully reserved at all times. It can request a big virtual memory space instead, and then during times of less memory-calling, the OS can allocate this to other processes.
### ii) \[2 marks]
What is demand paging? Can you also describe an alternative to demand paging?
#### My Answer (probably wrong)
Demand paging is when we do not preempt pages, simply allocating them into frames when needed. 
##### From BetterInformatics
Demand paging is when we transfer frames from disk only when they are specifically requested by the system. An alternative to this would be loading a full process's frames into memory while it is active.
### iii) \[2 marks]
What is the working set of a process? Mention at least a use-case of the working set size.
#### My Answer
The working set of a process is a set of pages for a process that is in memory *at all times*. A use case would be constantly used library functions, which would remove any paging-based overhead from a core segment of the program (if not in the working set, these may be paged so often to incur *thrashing*).
## b)
Suppose an OS implements virtual memory paging. A virtual address is $40$ bits long and a page comprises $1KB$ $(2^{10}) \text{ bytes}$. A hierarchical *three level* page table is used. The first-level, second-level, and third-level page tables each have 1024 ($2^{10}$) entries respectively.

If a process uses the first 2GB ($2^{31}$) of its virtual address space, how much space would be occupied by *the page tables for that process* when:
### i) \[2 marks]
Each page table entry occupies 4 bytes?
#### My Answer
- We need to address $\frac{2^{31}}{2^{10}} = 2^{21}$ pages.
- If each page table entry is 4 ($2^{2}$) bytes, then we can store $\frac{2^{10}}{2^{2}}=2^{8}$ entries in each table.
- If we use $2^{8}$ entries in second and third-level, we only need to use $2^{5}$ entries in the first table.
- Since this first-level table is the only one in memory at all times, we use $2^{5}$ in the first-level table. It takes up $2^{6} (128)$ bytes
### iii) \[2 marks]
Single level this time
#### My Answer
Each page is $2^{10}$ bytes, so we have $2^{21}$ pages. $2^{23}$ bytes in total, and for 8 bytes $2^{24}$
## c)
![[Pasted image 20240423212810.png]]
do fifo lru and second chance
#### My Answer
##### FIFO
- (B) B
- (A) B A
- (D) B A D
- (B) *in list*
- (F) B A D F
- (D) *in list*
- (D) *in list*
- (C) A D F C
- (H) B F C H
- (B) *in list*
- (A) F C H A
- (A) *in list*
- (G) C H A G
- (A) *in list*
- (B) H A G B
- (G) *in list*
- (D) A G B D
- (F) G B D F
- (F) *in list*
- (E) B D F E
Page Faults: 13
##### LRU
- (B) B
- (A) B A
- (D) B A D
- (B) *in list*
- (F) B A D F
- (D) *in list*
- (D) *in list*
- (C) (LRU:A) B D F C
- (H) (LRU:B) D F C H
- (B) (LRU:F) D C H B
- (A) (LRU:D) C H B A
- (A) *in list*
- (G) (LRU:C) H B A G
- (A) *in list*
- (B) *in list*
- (G) *in list*
- (D) (LRU:H) B A G D
- (F) (LRU:A) B G D F
- (F) *in list*
- (E) (LRU:B) G D F E
##### SC
*Assuming reference bit of 1, not 8bits like the book says*

| *Page Requested* | Pages in memory         |
| ---------------- | ----------------------- |
| B                | B {1}                   |
| A                | B {1} A {1}             |
| D                | B {1} A {1} D {1}       |
| B                | B {1} A {1} D {1}       |
| F                | B {1} A {1} D {1} F {1} |
| D                | B {1} A {1} D {1} F {1} |
| D                | B {1} A {1} D {1} F {1} |
| C                | A {0} D {0} F {0} C{1}  |
| H                | D {0} F {0} C {1} H {1} |
| B                | F {0} C {1} H {1} B {1} |
| A                | C {1} H {1} B {1} A {1} |
| A                | C {1} H {1} B {1} A {1} |
| G                | H {0} B {0} A {0} G {1} |
| A                | H {0} B {0} A {1} G {1} |
| B                | H {0} B {1} A {1} G {1} |
| G                | H {0} B {1} A {1} G {1} |
| D                | B {0} A {0} G {0} D {1} |
| F                | A {0} G {0} D {0} F {1} |
| F                | A {0} G {0} D {0} F {1} |
| E                | G {0} D {0} F {0} E {1} |

## d)
![[Pasted image 20240424102249.png]]
## i)
- block size 8kb = 8000 bytes
- 13 direct blocks = $8\text{KB} \times 13$ bytes at this level
- 1 single indirect blocks = $(8\text{KB} / 4) \times 8\text{KB}$ bytes at this level.
	- this is because we can address $(8\text{KB} / 4)$ direct blocks
- 1 double indirect blocks = $(8\text{KB} / 4) \times (8\text{KB} / 4) \times 8KB$
	- we can address $(8\text{KB} / 4)$ single indirects, which can address $(8\text{KB} / 4)$ each
- 1 triply indirect block = $((8\text{KB} / 4))^{3} \times 8\text{KB}$
	- you get the idea

Max file size = $((8\text{KB} / 4)^{3} + (8\text{KB} / 4)^{2} + (8\text{KB} / 4)) \times 8\text{KB}$
### ii)
we don't know
### iii)
How many disk accesses are required to access either the byte in position
1. $17$?
2. $23,423,956$?
#### My answer
for 17, we only need one disk access. We can access the first direct block, which contains the byte in that position ($17\times4<8\text{KB}$).
for 23,423,956, we need to work out at what position contains this.
- it's not the first 13 blocks, as $\frac{8\text{KB}}{{4}}<23,423,956$ 
- it's not in the single indirects which can access 64KB
- double indirects can access 64MB/4 = 16mb

So because our byte is roughly in 2mb range, and 64KB < 2MB < 16MB, we'd be accessing the doubly indirect blocks which leave us with *three disk accesses* (for each tier of blocks).