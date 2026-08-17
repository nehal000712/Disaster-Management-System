# Disaster-Management-System
🚨 Disaster Response Management System

An Emergency Coordination Simulator Implemented in C

![Language](https://img.shields.io/badge/Language-C-blue)
![Standard](https://img.shields.io/badge/Standard-C99-blue)
![Data Structures](https://img.shields.io/badge/Data%20Structures-Linked%20List%20%7C%20Queue%20%7C%20Stack%20%7C%20BST-orange)
![License](https://img.shields.io/badge/License-MIT-green)

A console-based Disaster Response Management System written in pure C, developed for the Data Structure Lab. The system simulates the coordination workflow of an emergency response team — registering affected people, logging resources, raising rescue requests, prioritizing by severity, dispatching help, and rolling back mistakes — using five different data structures, each chosen for the specific problem it solves best.

---

## 📌 Overview

Rather than relying on one generic structure for everything, this project deliberately combines several:

- **Singly linked lists** manage the dynamic, unordered registries of people and resources
- A **queue** enforces first-come-first-served ordering of rescue requests
- A **selection-sort routine** lets that order be overridden by severity
- A **stack** records dispatch history so a dispatch can be rolled back
- A **binary search tree** aggregates affected-people and resource counts by location

---

## ⚡ Core Architecture

```
                     +-----------------------+
                     |        main()         |
                     |   (menu / controller)  |
                     +-----------+-----------+
                                 |
          +----------------+----------------+----------------+----------------+
          |                |                |                |                |
    +-----v-----+   +------v------+  +-----v-----+   +------v------+  +------v------+
    |  Person   |   |  Resource   |  |  Request  |   |  StackNode  |  | LocationNode|
    | (Linked   |   |  (Linked    |  |  (Queue)  |   |  (Stack -   |  |    (BST)    |
    |  List)    |   |   List)     |  |           |   |  rollback)  |  |             |
    +-----------+   +-------------+  +-----------+   +-------------+  +-------------+
          |                |                |                |                ^
          +----------------+----------------+----------------+----------------+
                    every add / dispatch / rollback updates the location BST
```

The menu controller owns all top-level pointers and routes each menu choice to the corresponding function on the relevant structure. Every operation that touches a location also updates the shared location BST, which acts as a derived, always-consistent summary index.

---

## 🧩 Data Structures & Complexity

| Requirement | Data Structure | Why |
|---|---|---|
| Store growing number of affected people | Singly linked list (`Person`) | Unknown count in advance; O(1) tail insertion |
| Store growing number of resources | Singly linked list (`Resource`) | Same rationale — incremental logging |
| Handle rescue requests FCFS + reprioritization | Linked queue (`QueueNode`) | Natural FCFS model, no fixed capacity |
| Undo the most recent dispatch | Linked stack (`StackNode`) | Rollback is inherently LIFO, O(1) |
| Aggregate/lookup totals by location | Binary search tree (`LocationNode`) | O(log n) lookup + alphabetical in-order listing |
| Prioritize requests by severity | Selection sort on queue nodes | Simple, in-place, easy to verify at lab scale |

```c
typedef struct Person {
    int id;
    char name[NAME_SIZE];
    char location[NAME_SIZE];
    int severity;
    struct Person *next;
} Person;

typedef struct Request {
    int id;
    char personName[NAME_SIZE];
    char location[NAME_SIZE];
    char need[NEED_SIZE];
    int severity;
} Request;

typedef struct LocationNode {
    char location[NAME_SIZE];
    int totalAffected;
    int totalResources;
    struct LocationNode *left;
    struct LocationNode *right;
} LocationNode;
```

### Complexity Summary

| Operation | Time | Space |
|---|---|---|
| Add person / resource (append) | O(n) | O(1) extra |
| Enqueue / Dequeue request | O(1) | O(1) extra |
| Push / Pop dispatch stack | O(1) | O(1) extra |
| Sort requests by severity | O(n²) | O(1) extra |
| BST insert-or-update / search | O(log n) avg, O(n) worst | O(1) extra |
| In-order location listing | O(n) | O(h) recursion |

---

## 🔄 Dispatch & Rollback Flow

Dispatch always re-sorts the queue by severity first, so the request dequeued is guaranteed to be the current highest-severity one — not simply the oldest.

**Dispatch**
```
Queue empty? --Yes--> "No request available"
      |No
      v
Sort queue by severity
      |
      v
Dequeue front request --> Push (dispatchId, request) onto Stack
      |
      v
Print confirmation, increment dispatchId
```

**Rollback**
```
Stack empty? --Yes--> "No dispatch history"
      |No
      v
Pop (dispatchId, request) off Stack
      |
      v
Enqueue request back into Queue --> Re-sort by severity
      |
      v
Print rollback confirmation
```

---



## 🖥️ Features / Menu Options

| # | Operation |
|---|---|
| 1 | Add affected person |
| 2 | View affected people |
| 3 | Add resource |
| 4 | View resources |
| 5 | Raise rescue request |
| 6 | View rescue requests |
| 7 | Sort requests by severity |
| 8 | Dispatch next request |
| 9 | Roll back most recent dispatch |
| 10 | Search a location |
| 11 | View aggregated location summary |
| 0 | Exit (runs full memory cleanup) |

---

## 🛠️ Build & Run

```bash
gcc -std=c99 -Wall -o disaster_response disaster_response_management_system.c
./disaster_response
```

---

## 🧠 Memory Management

Every node type (`Person`, `Resource`, `QueueNode`, `StackNode`, `LocationNode`) is heap-allocated with `malloc()` and NULL-checked before use. On exit, five dedicated cleanup routines — `freePeople()`, `freeResources()`, `freeQueue()`, `freeStack()`, `freeTree()` — release every allocated node exactly once.

---

## ⚠️ Known Limitations

- Location matching is case-sensitive and exact (`strcmp`) — `"Mirpur"` ≠ `"mirpur"`
- BST is **not self-balancing**; sorted-order insertion degrades lookup to O(n)
- No persistence — all data is lost on exit (in-memory only)
- Fixed-size buffers (`NAME_SIZE = 50`, `NEED_SIZE = 100`) silently truncate longer input

---

## 👥 Team

| Name | Student ID |
|---|---|
| Samina Tasnim Rahi | 253-15-179 |
| Tanzila Khan Mazlish | 253-15-402 |
| Tasnova Aziz Tanisha | 253-15-327 |
| Dilruba Chowdhury Tithi | 253-15-328 |

**Instructor:** Sharbajit Paul Bappy
**Course:** Data Structure Lab

---

## 📚 References

1. Kernighan, B. W., & Ritchie, D. M. (1988). *The C Programming Language* (2nd ed.). Prentice Hall.
2. Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
3. Weiss, M. A. (2013). *Data Structures and Algorithm Analysis in C* (2nd ed.). Pearson.
4. Deitel, P., & Deitel, H. (2015). *C: How to Program* (8th ed.). Pearson.

---

## 📄 License

This project is licensed under the MIT License.
