# Runtime Adaptive Systems

A Linux runtime control system that observes low-level signals (memory, CPU, I/O) and adapts application behavior to reduce performance bottlenecks.

## Idea

Programs often slow down due to changing system conditions like memory pressure, cache misses, or I/O delays. This project builds a feedback loop:

observe → detect → act

The system monitors runtime signals and applies control decisions such as adjusting concurrency or throttling execution.

## Scope

- Collect signals: page faults, CPU usage, I/O wait
- Detect bottlenecks at runtime
- Apply simple control policies
- Measure impact on performance

## Architecture

Application  
↓  
Probes (eBPF / perf)  
↓  
Signal Collector  
↓  
Controller  
↓  
Runtime Actions  

## Stack

- C / C++
- Linux (eBPF, perf)
- Python (analysis)

Current Status

Basic setup is working. Signals are being collected and printed.
Control logic is still simple and being tuned.

Experiments
Memory-heavy workload shows consistently high major page faults
This clearly indicates strong memory pressure during continuous allocation
System is able to detect this, next step is improving how it reacts

## Status

Initial setup

Current Status

Basic setup is working. Signals are being collected and printed.
Control logic is still simple and being tuned.

## Experiments

Basic memory workload shows sustained high major page fault rate, indicating strong memory pressure under continuous allocation.

Memory-heavy workload shows consistently high major page faults
This clearly indicates strong memory pressure during continuous allocation
System is able to detect this, next step is improving how it reacts
