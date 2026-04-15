# Predictive Disk Scheduling Simulation in C

## 📌 Overview
This project is done by Sourav Kundu and Anubhab Neogi.
We implemented and compared multiple **disk scheduling algorithms**. As part of the project we evaluate their performance based on **total seek operations** and show how different algorithms impact disk head movement.

---

## Algorithms Implemented

* **FCFS (First Come First Serve)**
  Processes requests in the order they arrive.

* **SSTF (Shortest Seek Time First)**
  Selects the request closest to the current head position.

* **SCAN (Elevator Algorithm)**
  Moves the head in one direction, servicing requests, then reverses after hitting end of the disk in that direction.

* **LOOK**
  Similar to SCAN but only goes as far as the last request in each direction.

* **Predictive Scheduling  ⭐**
  A heuristic-based approach that:

  * Uses recent request history
  * Estimates movement direction
  * Applies a scoring function to select the next request

---

## Predictive Algorithm Insight

The predictive algorithm attempts to improve performance by:

* Tracking **recently arrived requests**
* Estimating **direction of movement**
* Using a **moving average** for prediction
* Applying penalties for direction mismatch

> Note: This is a heuristic-based approximation of predictive scheduling.

---

## 🎲 Input Modes

### 1. Auto-Generated Mode (Default)

* Random number of requests
* Mix of:

  * **Sequential patterns (70%)**
  * **Random requests (30%)**

### 2. Manual Input Mode

Uncomment the following line in `main()`:

```c
input_req(base, &n, &head);
```

Then provide:

* Number of requests
* Request queue
* Initial head position

---

## 📊 Output

The program displays:

* Total number of requests
* Initial head position
* Request queue
* Served order for predictive algorithm
* **Total seek operations** for each algorithm

Example:

```
=== Disk Scheduling Simulation ===
Total Requests: 9
Initial Head Position: 67
Request Queue:
[ 20 22 24 26 28 56 38 198 53 ]
PREDICTIVE Order: 67 -> 56 -> 20 -> 22 -> 24 -> 26 -> 28 -> 38 -> 53 -> 198 
--- Performance Results ---
Algorithm       Total Seek Operations
----------------------------------
FCFS            406
SSTF            225
SCAN            311
LOOK            309
PREDICTIVE      225
----------------------------------
```

---

## 🛠️ How to Compile & Run

### Compile:

```bash
gcc predictive_disk_scheduling.c -o  predictive_disk_scheduling  
```

### Run:

```bash
./predictive_disk_scheduling  
```

---

## 📁 Project Structure

```
predictive_disk_scheduling.c   # Main code
README.md           # User manual
```
