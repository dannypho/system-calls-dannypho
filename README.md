# xv6 System Call Extension

This project extends the `xv6` operating system to include priority-based scheduling, new system calls, and a custom `ps` user application. The modifications showcase my understanding of operating system internals, including process control, system call implementation, and kernel-space to user-space communication.

---

## Features Implemented

### 1. **Process Control Block Enhancements**
The process control block (`struct proc`) was modified to include the following fields:
- `real_priority`: The default priority of the process (range: `-20` to `20`).
- `effective_priority`: The current priority of the process, which may change due to aging or manual adjustments.

Both fields are protected by the `p->lock` to ensure thread safety during access and modification.

---

### 2. **System Calls**

`int setPriority(int pid, int priority)`
- **Purpose**: Sets the `real_priority` of a process identified by its PID.
- **Validation**: Ensures the priority is within the range `-20` to `20`.
- **Return Value**:
  - `0` on success.
  - `-1` if the priority is invalid or the PID does not exist.

`int setEffectivePriority(int pid, int priority)`
- **Purpose**: Sets the `effective_priority` of a process identified by its PID.
- **Validation**: Ensures the priority is within the range `-20` to `20`.
- **Return Value**:
  - `0` on success.
  - `-1` if the priority is invalid or the PID does not exist.

`int getpinfo(struct pstat *)`
- **Purpose**: Retrieves information about all processes in the system, including their names, states, priorities, and accumulated ticks.
- **Validation**: Checks the pointer passed from user space for validity and safely copies data from kernel space to user space.
- **Return Value**:
  - `0` on success.
  - `-1` if the pointer is invalid.

---

### 3. **Custom `ps` Command**
A new user-space application, `ps`, was implemented to display process information using the `getpinfo` system call. The output includes:
- Process name
- PID
- State (e.g., RUNNING, SLEEPING)
- Real priority

**Example Output:**


```bash
NAME    PID     STATUS      PRIORITY
init    1       SLEEPING    0
sh      2       SLEEPING    0
test    4       RUNNING    -10
ps      6       RUNNING     20
```

## Files Modified

__Kernel Files__

* proc.h: Added fields to struct proc.

* proc.c:

  * Implemented the logic for new system calls.

  * Enhanced process scheduling logic to incorporate priorities (planned).

* sysproc.c: Added system call implementations.

__User Space__

* ps.c: New user application to display process information.

## How to Build and Run

1. Build the Kernel
```bash
make
```

2. Run xv6
```bash
make qemu
```

4. Test the ps Application

Inside the xv6 shell:
```bash
ps
```
5. To exit xv6
```bash
ctrl-a x
```

