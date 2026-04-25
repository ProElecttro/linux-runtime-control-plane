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

## Status

Initial setup
