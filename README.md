# FastKV

## Redis-Inspired In-Memory Key-Value Store

FastKV is a Redis-inspired in-memory key-value store built from scratch in C++17. The project explores systems programming concepts including TCP networking, protocol parsing, concurrent execution, efficient in-memory storage, expiration policies, and persistence.

This project is being built from scratch as a systems programming and backend engineering learning project.

Planned features:
- TCP socket server
- RESP protocol parser
- epoll-based event loop
- Concurrent command execution
- Sharded in-memory key-value store
- TTL expiry
- Append-Only File (AOF) persistence

## Build

```bash
cmake -B build
cmake --build build
./build/fastkv
```