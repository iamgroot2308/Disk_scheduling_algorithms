# 💽 Disk Scheduling Simulation in C

## 📌 Overview

This project implements and compares multiple **disk scheduling algorithms** using a simulation written in C. It evaluates their performance based on **total seek operations** and demonstrates how different strategies impact disk head movement.

---

## ⚙️ Algorithms Implemented

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

## 🧠 Predictive Algorithm Insight

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
predictive_disk_scheduling.c   # Main simulation code
README.md           # Project documentation
```

---

## 🚀 Features

* Modular implementation of scheduling algorithms
* Custom predictive heuristic
* Random workload generation
* Performance comparison across algorithms

---

## ⚠️ Limitations

* Predictive algorithm does not include **arrival time modeling**
* All requests are assumed to be available initially

---

## 🔮 Future Improvements

* We can add **arrival time** for the requests
* Implement **CSCAN and CLOOK**
* Improve predictive model using trend analysis

---

## 👨‍💻 Author

Sourav Kundu

Anubhab Neogi

---

## 📜 License

This project is for educational purposes.
