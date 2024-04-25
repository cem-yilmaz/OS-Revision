*This paper contains more theory questions which I'm trying to focus on as opposed to the more busywork questions of [[2022 (May) - My answers|'22]] and [[2023 (May) - My Answers|'23]]*
# 1
## a) \[3 marks]
State three advantages of loadable kernel modules
### My Answer (pre-looking at notes)
1. LKMs allow for independent developers to work on optional features for a kernel *without* having it be approved into the core kernel.
2. LKMs allow for easy debugging (*needed to look at notes at this point*)
### My Answer (after looking at notes)
2. Modules can talk to other modules easily without overhead, since this is managed through the kernel. This allows developers to provide "library-like" modules, so other developers can "piggyback" of these interfaces.
3. Modules can be easily inserted/removed, which allows developers to *easily* choose what (extra) functionality they want from an OS without worrying about dependencies (since *modules are isolated*).
## b) \[8(!) marks]
Briefly define the notion of process. Sketch a diagram of process states and transitions, giving one-sentence definitions of each state and transition. 
### My Answer (did have to look at notes here)
A process is a program *in execution*. It has it's own virtual memory space and sends instructions for the CPU to execute.

A process has many states:
- **New**
	- A **new** process has just been created; it has not yet finished setting up all the prerequisites. Once this has occurred, the process is **ready**.
- **Ready**
	- A **ready** process is *waiting* to be scheduled into execution by the CPU. Once this has occurred, the process is **running**.
- **Running**
	- A **running** process *is currently having it's instructions **executed** by the CPU*. If the CPU is *interrupted*, this process then becomes **ready**. If the CPU *reaches an exit signal*, then the process is **terminated**. If the process needs to *run an I/O request*, then it is **waiting**.
- **Waiting**
	- A **waiting** process is *still running but **not doing any CPU execution** until I/O has finished*. Once this has finished, the process becomes **ready**.
- **Terminated**
	- A **terminated** process *has finished executing*, and has no other further states. 
## c) \[5 marks]
State the criteria to be satisfied by solutions to the mutual exclusion problem
### My Answer
*I'm just going to assume this is referring to the conditions for a deadlock? It's the only thing in the current course that really makes sense.*

Deadlocking occurs when the following 4 criteria *are all true at once*:
- **Mutual Exclusion**: At least one resource must be set to only be accessed by one process at a time; if another process wishes to access it then *it must wait*.
- **Circular requests**: Process 1 needs resource 2, but that's held by process 2 which is waiting on resource 3, but that's held... by process $N-1$, which is waiting on resource $N$, but that's held by Process 1!
- **Non-preemption**: Preempting *cannot release* resources from processes - they must be voluntarily released
- **Hold and wait**: A process must be *holding* a resource **and** waiting on *another held resource* (by another process)
## d) \[3 marks]
Recall that a `TestAndSet` function operates on a word of memory at address `addr` as follows:
- it reads `*addr` (contents of `addr`)
- sets `*addr` to 1
- returns the previous value
*all in one atomic operation*.

Using `C`, show how to use `TestAndSet` to protect a section of critical code `X` shared between several processes
### My Answer
**Process 1**
```c
	int prev_val = TestAndSet(addr);
	X();
	*addr = prev_val	
```
**Process 2**
```c
	while (TestAndSet(addr) = 1) {
		// wait for Process 1 to finish
	}
	X();
```
## e) 
Now consider a standard linked list of integers:
```c
typedef struct {
	int data; /* data stored in this node */
	node *next; /* pointer to the next node in the list */
} node;
typedef node *listl
```
Let `myList` be a global, shared variable containing a list. We can insert a new data element `d` at the front of the list by `insert(& mylist,d)`, where `insert` is the following standard procedure:
```c
void insert(list *list_ptr, int d) {
	node *newnode_ptr = malloc(sizeof(node));
	newnode_ptr->data = d;
	newnode_ptr->next = *list_ptr;
	*list_ptr = newnode_ptr;
}
```
## f)
Show, by considering two threads each calling insert on the same list, that this code has *race conditions*.
### My Answer
$T_{1}$ and $T_{2}$ both call `insert(list_2, 1)` and `insert(list_2, 2)` respectively. A CPU may schedule it so that the creates `newnode_ptr` for $T_{1}$ and then runs the `malloc` and `data=` commands for $T_{1}$. $T_{2}$ could technically have only the `node *newnode_ptr` allocated, and then when $T_{1}$ runs more, it'd put it's *own `list_ptr`* in $T_{2}$'s node.
## g)
Using `TestAndSet`, modify the insert code so that it is thread-safe
### My Answer
```c
void insert(list *list_ptr, int d) {
	node *newnode_ptr = malloc(sizeof(node));
	while (TestAndSet(newnode_ptr) = 1) {
		asm(nop); //wait for it not to be
	}
	int prev_val = TestAndSet(newnode_ptr);
	newnode_ptr->data = d;
	newnode_ptr->next = *list_ptr;
	*list_ptr = newnode_ptr;
	*newnode_ptr = prev_val;
}
```
# 2
## a)
Buddy allocation is used by the kernel to allocate memory for its own use. Suppose that 32 pages of memory are being allocated via the buddy algo- rithm, with a minimum block size of 1 page and no maximum block size. Assume that all memory is initially free. Describe the evolution of the memory allocation structure as the following sequence of events happens. When a request cannot be satisfied, say so, and assume the requesting process is killed.
1. Process P1 requests 10 pages of memory
2. Process P2 requests 5 pages of memory
3. Process P3 requests 1 page of memory
4. Process P4 requests 7 pages of memory
5. Process P1 terminates
6. Process P5 requests 3 pages of memory
7. Process P3 terminates
### My Answer
1. We allocate 16 pages to P1. We now have 16 pages left
2. We allocate 8 pages to P2. We now have 8 pages left
3. We allocate 2 pages to P3. We now have 6 pages left
4. We would need to allocate 8 pages to P4. We only have 6 and this cannot be satisfied, so P4 is terminated. We now have 6 pages left
5. We free the 16 pages held by P1. We now have 22 pages left
6. We allocate 4 pages to P5. We now have 18 pages left
7. We free the 2 pages allocated to P3. We now have 20 pages left
## b)
Paging is used on modern systems to prevent fragmentation
### i)
Draw a labelled diagram explaining how a logical address is translated into a physical address with paging hardware. You should include the TLB and page table in your diagram.
#### My Answer
![[b)i).png]]
### ii)
In a 128-bit machine with 8KB pages, how large is the page table?
#### My Answer
Assuming a *single-level page table*:
- each address takes up $128$-bits, so we can access $2^{128}$ locations
- $8\text{KB} = 8 \times 1024 = 8 \times 2^{10} = 2^{3} \times 2^{10} = 2^{13}$ 
- Therefore, the amount of pages we have is $\frac{2^{128}}{2^{13}} = 2^{115}$
- The size of the page table depends on the size of the page table entry $\text{PTE}$, so the page table is $2^{115}\times \text{PTE}$ in size.
- ChatGPT tells me the PTE would likely be 128 bits in size (though is not necessary), so under this assumption
	- $2^{115} \times 16\text{ (128 bits in bytes)} = 2^{115}\times2^{4} = 2^{119}$ bytes in size
### ii)
Explain one method to reduce this size
#### My Answer
Using a *multi-level page table* solves this issue. The highest level page table is kept in memory at all times, and instead points to *lower-level page tables* (which may either point to the actual addresses or *further lower-level* tables). This results in marginally slower lookup (due to having to pull these addresses out of secondary storage when they are accessed), but drastically reduces the size.
## c)
Virtual memory is a mechanism that supports larger logical memory than physical memory by using physical memory as a page cache to disk. Consider a system with only 4 frames i.e. only 4 pages can sit in memory. 

A process iteratively accesses six pages in the following order:  

 > 1,2,3,4,5,6,5,6,5,6

Initially the six pages are stored only on disk.
### i)
If the OS employs the MRU algorithm how many page faults will there be after 1 iteration and 1 iterations?
#### My answer
*I'm going to assume that MRU means we replace the last page we used. This isn't covered on the course currently from what I can tell. The **Most Frequently Used (MFU)** is covered, but that replaces the page most frequently used and is different. I'm putting this disclaimer here because personally this algorithm is a terrible fit for this question and looks stupid.*

1. We **pagefault** 1 into memory
2. We **pagefault** 2 into memory
3. We **pagefault** 3 into memory
4. We **pagefault** 4 into memory. Memory is now full.
5. We **take 4 out of memory** and **pagefault 5** into memory
6. We **take 5 out of memory** and **pagefault 6** into memory
7. We **take 6 out of memory** and **pagefault 5** into memory
8. We **take 5 out of memory** and **pagefault 6** into memory
9. We **take 6 out of memory** and **pagefault 5** into memory
10. We **take 5 out of memory** and **pagefault 6** into memory

1 PF after 1iter, 10 after 10iter
### ii)
#### My Answer
*I'm taking this to mean **optimal page replacement**, as in the algorithm that reduces Belady's anomaly as much as possible.*

1. We **pagefault** 1 into memory
2. We **pagefault** 2 into memory
3. We **pagefault** 3 into memory
4. We **pagefault** 4 into memory. Memory is now full.
5. We **take 1 out of memory** and **pagefault 5** into memory
6. We **take 2 out of memory** and **pagefault 6** into memory
7. 5 is in memory
8. 6 is in memory
9. 5 is in memory
10. 6 is in memory

1 PF after 1iter, 6 PF after 6iter
### iii)
Least recently used (LRU). This algorithm would have poor performance in cases where we have a page used for (e.g.) a library function that gets constantly periodically accessed, at a such intervals it has to be swapped in every single time.
## d)
Briefly describe one possible requirement for an architecture to be virtualisable
### My Answer
A distinction between
## e)
Why is paravirtualisation a more efficient form of hardware virtualisation?
### My Answer
Paravirtualised OSes *know* they're being virtualised, and as such can change their requests under the hood to be kinder on the VMM. This results in less VMM overhead and is a more efficient form of hardware virtualisation.
## f)
What environment would you choose to deploy paravirtualised virtual machines?
### My Answer
Type 0 hypervisors would be an excellent environment as, due to the less overhead, these VMs can run almost natively on the hardware the host is on.
# 3
## a)
List (one sentence definitions) 3 criteria that might be used to evaluate process scheduling algorithms
### My Answer
- **Average Waiting Time** - the average time a process takes *from being sent in the ready queue **to** being first executed by the CPU*
- **Average Turnaround Time** - the average time a process takes *from being sent in the ready queue **to** finishing it's full execution*
- **Average Mid-schedule waiting time** - the average time a process **waits** *after it has been partially executed once *
	- basically, time spent waiting for other scheduled processes that have interrupted it after it began execution
## b) \[6 (??!!??!??!!!) marks]
Explain briefly the round-robin, static priority, and feedback (dynamic priority queues)
### My Answer
- **Round Robin** - we assign the CPU a certain *time quanta*. The CPU then goes through the ready queue (*irrespective of priority*(?)) and works on the first process **for that time quanta**. If it finishes within this time, it moves to the next. If it *doesn't* finish, it *suspends* the process and moves to the next. This continues with all active (unfinished) processes until all are complete.
- **Static Priority** - The CPU executes processes *in order of their priority*.
- **Feedback** - (*I'm going to assume this is pre-emptive priority?*) The CPU starts executing processes like **static priority.** At any time, the priority queue can be *pre-empted* by a new process. This process may be the most important priority in the whole queue and as such *is pushed to the front*. The CPU then rechecks the queue *after executing the whole process* and starts executing whatever's at the front.
## c)
Consider a single-processor system in which processes are pre-emptively scheduled with a quantum of 1 unit, and are assigned priorities on creation which do not change. Suppose that three processes are created with the following properties:
- P1 starts at time 0, has priority 1 (low), and needs to execute for 5 quanta to complete its work.
- P2 starts at time 3, has priority 3 (high), and needs 4 quanta to complete its work. 
- P3 starts at time 4, has priority 2 (medium), and needs 10 quanta to complete its work.
#### My Answer

|     | $P1$ (low) | $P2$ (high) | $P3$ (med) |
| --- | :--------: | :---------: | :--------: |
| 0   |     X      |             |            |
| 1   |     X      |             |            |
| 2   |     X      |             |            |
| 3   |            |      X      |            |
| 4   |            |             |     X      |
| 5   |     X      |             |            |
| 6   |            |      X      |            |
| 7   |            |             |     X      |
| 8   |  X - DONE  |             |            |
| 9   |            |      X      |            |
| 10  |            |             |     X      |
| 11  |            |  X - DONE   |            |
| 12  |            |             |     X      |
| 13  |            |             |     X      |
| 14  |            |             |     X      |
| 15  |            |             |     X      |
| 16  |            |             |     X      |
| 17  |            |             |     X      |
| 18  |            |             |  X - DONE  |
## d)
Now consider a system as in the prev part, but with the following behaviour
- After executing for 1 quantum, $P1$ requests exclusive access to resource $R$
- Once it obtains $R$, it executes for 3 quanta before releasing it and continuing with its remaining quantum of execution
- After executing for 1 quantum, $P3$ requests exclusive access to resource $R$
- Having obtained it, it executes for 1 quantum before releasing it and continuing
- When a process is waiting for a resource that is not available, it is blocked
#### My Answer
|     |  $P1$ (low)  | $P2$ (high) | $P3$ (med) |
| --- | :----------: | :---------: | :--------: |
| 1   |      X       |             |            |
| 2   |    X (R)     |             |            |
| 3   |    X (R)     |             |            |
| 4   |     (R)      |      X      |            |
| 5   |     (R)      |             |     X      |
| 6   |    X (R)     |             |            |
| 7   |     (R)      |      X      |            |
| 8   | X (R) - DONE |             |  BLOCKED   |
| 9   |              |      X      |            |
| 10  |              |             |   X (R)    |
| 11  |              |  X - DONE   |            |
| 12  |              |             |     X      |
| 13  |              |             |     X      |
| 14  |              |             |     X      |
| 15  |              |             |     X      |
| 16  |              |             |     X      |
| 17  |              |             |     X      |
| 18  |              |             |     X      |
| 19  |              |             |     X      |

Finishing times are the same apart from $P3$ which is delayed by 1. This might be a problem because $P3$ was blocked from using $R$ one cycle. This resource-holding could result in processes running sequentially regardless of scheduler fairness aims, or even worse, deadlocks - and these are a problem.