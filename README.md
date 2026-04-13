*This project has been created as part of the 42 curriculum by nelhansa.*

**Description**
- Codexion implements a variation of the classic Dining Philosophers problem. "Coders" (threads) compete for shared resources called "dongles" and must execute a repeated cycle of: compile → debugging → refactoring. The goal of the project is to model concurrent resource allocation while preventing deadlocks, handling starvation, enforcing cooldowns on shared resources, and detecting "burnout" (a coder that waits too long without compiling).

**Instructions**
- Compile:

```bash
makeThe very first line must be italicized and read: This project has been created as part
of the 42 curriculum by <login1>[, <login2>[, <login3>[...]]].
• A “Description” section that clearly presents the project, including its goal and a
brief overview.
• An “Instructions” section containing any relevant information about compilation,
installation, and/or execution.
• A “Resources” section listing classic references related to the topic (documen-
tation, articles, tutorials, etc.), as well as a description of how AI was used —
specifying for which tasks and which parts of the project.
```

- Run (arguments):

```text
./codexion <nb_coders> <time_to_burnout(ms)> <time_to_compile(ms)> <time_to_debug(ms)> <time_to_refactor(ms)> <must_compile> <dongle_cooldown(ms)> <fifo|edf>
```

- Example:

```bash
./codexion 8 800 200 200 200 2 50 fifo
```

This starts 8 coders, each with a burnout timeout of 800 ms, compile/debug/refactor durations of 200 ms each, a goal of 2 compile cycles per coder, dongles cooldown 50 ms, and FIFO scheduler.

**Resources**
- Dining Philosophers problem — classic concurrency exercise (Wikipedia)
- POSIX Threads (pthreads) documentation — GNU libc manual / man pages
- Articles on deadlocks and Coffman conditions (mutual exclusion, hold-and-wait, no preemption, circular wait)
- Tutorials on condition variables and producer-consumer patterns


**Blocking cases handled**
- Deadlock prevention:
  - Dongles (shared resources) are locked in a consistent order (or via scheduler-mediated acquisition) to avoid circular wait when possible.
  - In the actions that acquire two dongles, the implementation attempts to lock dongles in an order that prevents classical circular deadlock patterns.
- Starvation prevention:
  - FIFO scheduler implements a queue so that waiting coders are served in order. The system restricts the number of simultaneous compilers to at most floor(nb_coders / 2) so that resource contention is bounded.
  - EDF scheduler selects coders with the earliest deadline to avoid imminent burnouts; the heap selection logic ensures the n coders with the smallest deadlines are given priority.
- Cooldown handling:
  - Each `t_dongle` tracks a `last_used` timestamp guarded by a lock and a `wait_cooldown()` is used before a dongle is locked again, ensuring the cooldown interval is respected.
- Precise burnout detection:
  - A monitor thread checks per-coder `last_compile` times. If any coder exceeds `time_to_burnout`, the monitor sets a global stop flag and broadcasts to all waiters so they can exit promptly and safely.
- Log serialization:
  - All console prints are protected by a `print_lock` mutex to avoid interleaved log lines and to preserve human-readable ordering of events.

**Thread synchronization mechanisms (how they are used)**
- `pthread_mutex_t`:
  - `t_dongle::mutex` — protects individual dongle access; only one coder may hold a dongle mutex.
  - `t_dongle::last_used_lock` — protects access to the dongle `last_used` timestamp so cooldown checks are thread safe.
  - `table->queue_lock` — protects the scheduler queue/heap and is used together with `table->cond` for wait/wake logic.
  - `table->print_lock` — serializes access to stdout to avoid mixed output.
  - `table->stop_lock` — protects the global `stop` flag used to request shutdown.
  - Per-coder locks: `compile_count_lock` and `last_compile_lock` protect per-coder counters and timestamps. These prevent race conditions when monitor and coder threads read/write the same fields.
  - `table->complite_sim_lock` — protects the aggregate counter (`complite_sim`) that tracks how many coders finished the required compile cycles.

- `pthread_cond_t table->cond`:
  - FIFO: threads push their id into `table->queue.data` and wait on `table->cond` until they are within the allowed window (first n entries). When a coder releases dongles, it pops the queue and broadcasts on `table->cond` to wake waiting coders.
  - EDF: threads are inserted into `table->heap` and wait on `table->cond` until they are among the n coders with earliest deadlines. After release, the monitor or releaser calls `pthread_cond_broadcast` to wake waiting threads so one (or more) can re-evaluate eligibility.

- How race conditions are prevented (examples):
  - Updating `last_compile` is done under `last_compile_lock`, and the monitor reads `last_compile` through a getter that locks the same lock. This avoids the monitor seeing torn/partially-updated values.
  - Incrementing `compile_count` is done with `compile_count_lock`. When a coder reaches `must_compile`, a protected update to `table->complite_sim` is performed under `complite_sim_lock` so the monitor can reliably detect completion.
  - Queue/heap operations (push/remove/add_to_heap/remove_min) are performed while holding `queue_lock`, guaranteeing consistent ordering of waiting coders and avoiding concurrent corruption.

**Design choices & notes**
- Two scheduler strategies are supported:
  - FIFO: fairness via queue order. Allows up to floor(nb_coders / 2) coders to acquire dongles simultaneously.
  - EDF: coders with the earliest deadline (closest burnout) are prioritized. The heap data structure is used for efficient selection.
- At least one coder is always allowed to proceed (guarding against divide-by-zero or floor-to-zero edge cases).
- The monitor is the single actor responsible for declaring burnout and global termination. It sets the `stop` flag and broadcasts to wake any waiters so they can exit cleanly.

**How to debug common hang/block issues**
- If threads appear to be stuck, check:
  - That `table->heap.size` and `table->queue.f/r` are correctly initialized and not corrupted.
  - That `broadcast_coder()` is called on `set_stop` and that waiting threads use the same `cond` and `queue_lock`.
  - The per-coder locks are initialized before thread creation.
- Useful commands for debugging:


**Contributors**
- nelhansa (author / main implementer)