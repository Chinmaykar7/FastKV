# RedisLite

RedisLite is a Redis-compatible concurrent in-memory key-value store built in C++17.

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
./build/redislite
