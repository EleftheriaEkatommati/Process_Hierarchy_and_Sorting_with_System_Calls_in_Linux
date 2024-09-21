# Project: mysort

## Overview
This project aims to familiarize you with system calls that create, manage, and coordinate processes in a hierarchical structure. The program, named `mysort`, sorts records from a binary file using a k-ary tree of cooperating processes.

## Objectives
- **Process Hierarchy**: Create a process hierarchy using `fork()`.
- **Inter-process Communication**: Use pipes and signals for communication between processes.
- **System Calls**: Utilize various system calls like `exec*()`, `mkfifo()`, `pipe()`, `open()`, `close()`, `read()`, `write()`, `poll()`, `wait()`, `select()`, `dup()`, `dup2()`, `kill()`, and `exit()`.

## Implementation Details
1. **Process Creation**: 
   - The root process (`mysort`) creates internal nodes and leaf nodes using `fork()`.
   - Internal nodes manage subsets of records and communicate results back to the root.

2. **Sorting**:
   - Leaf nodes sort subsets of records using two different sorting programs.
   - Sorting is based on the surname, first name, and registration number.

3. **Communication**:
   - Internal nodes and the root process communicate using pipes (both unnamed and named).
   - Signals (`USR1` and `USR2`) are used to indicate task completion.

## Example Command
```bash
./mysort -i DataFile -k NumofChildren -e1 sorting1 -e2 sorting2
```

## File Structure:
The document contains several files: main.c, Processes.c, Processes.h, sorting1.c, and sorting2.c.
## Main Function:
The main function opens a file, reads records, allocates necessary space, and processes command line inputs. It then calls the Coordinator_Splitter function to start the process.
## Coordinator_Splitter Function:
This function sets up communication between processes using pipes, divides records among splitters, and creates child processes using fork(). Each child process handles a portion of the records.
## Sorting Process:
Splitters distribute records to sorters, which sort the data and write the sorted results back to the parent process. The parent process merges the sorted results to produce the final output.