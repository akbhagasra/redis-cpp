# Redis in C++

Implementation of a Redis-like in-memory key-value store from scratch in C++. Implements the [RESP (Redis Serialization Protocol)](https://redis.io/docs/latest/develop/reference/protocol-spec/) for client-server communication, making it fully compatible with standard Redis tools like `redis-cli` and `redis-benchmark`.

## Highlights

- ~95% throughput of official Redis (215K SET/s, 234K GET/s)
- Thread-safe with mutex-protected store and background TTL cleanup
- Full RESP protocol - works with standard tools like redis-cli and redis-benchmark
- Multi-stage Docker build
- Three build modes: default, release (no logging), debug

## Supported Commands

| Command | Usage | Description |
|---------|-------|-------------|
| [PING](https://redis.io/docs/latest/commands/ping/) | `PING` | Test connection |
| [SET](https://redis.io/docs/latest/commands/set/) | `SET key value [EX seconds] [PX milliseconds]` | Set key with optional TTL |
| [GET](https://redis.io/docs/latest/commands/get/) | `GET key` | Get value by key |
| [DEL](https://redis.io/docs/latest/commands/del/) | `DEL key [key ...]` | Delete one or more keys |
| [ECHO](https://redis.io/docs/latest/commands/echo/) | `ECHO message` | Echo back a message |
| QUIT | `QUIT` | Close the connection |

## Source Code Layout

| File | Purpose |
|------|---------|
| `src/main.cc` | Starts the logger and the TCP server |
| `src/socket_manager.cc` | Handles socket creation, binding, listening and accepting clients |
| `src/input_parser.cc` | Parses raw RESP bytes into a command and its arguments |
| `src/command_handler.cc` | Runs the right handler for each command and sends back the response |
| `src/store.cc` | The key-value store - thread-safe map with TTL and a background cleanup thread |
| `src/logger.cc` | Writes timestamped logs to a file |
| `include/configs.h` | All configuration in one place |

## Key Design Choices

- **Thread-per-connection** - spawns a detached `std::thread` per client. Chose this over an event loop for simplicity
- **Dual TTL expiry** - lazy expiry on `GET` (checks timestamp before returning) + active background thread that sweeps expired keys every second. Prevents stale keys from accumulating in memory
- **Compile-time log stripping** - `RELEASE_MODE` flag compiles all `LOG_*` macros to no-ops, eliminating mutex contention and I/O on the hot path.

## Building

```bash
make            # default build (redis.out)
make release    # release build, no logging (redis.release.out)
make debug      # debug build, works with breakpoints (redis.debug.out)
make clean      # remove all build artifacts
```

### Docker

```bash
docker build -t redis .
docker run -p 6379:6379 redis
```

Uses a multi-stage build: `gcc:14` for compilation, `alpine:3.20` for the runtime image.

## Connecting

Requires `redis-cli`. Install via [redis.io](https://redis.io/docs/latest/operate/oss_and_stack/install/install-redis/) or:

```bash
# macOS
brew install redis

# Ubuntu/Debian
sudo apt install redis-tools
```

### Usage

```bash
redis-cli
127.0.0.1:6379> PING
PONG
127.0.0.1:6379> SET name hello
OK
127.0.0.1:6379> GET name
"hello"
127.0.0.1:6379> SET token abc EX 30
OK
127.0.0.1:6379> DEL name token
(integer) 2
127.0.0.1:6379> QUIT
```

## Benchmark

```bash
redis-benchmark -t SET,GET -n 100000 -q
```

**Official Redis**
```
SET: 226244.34 requests per second
GET: 236966.83 requests per second
```

**This project**
```
SET: 215517.25 requests per second
GET: 234192.05 requests per second
```

## Scope for Improvement

- **Event loop (epoll/kqueue)** - replace thread-per-connection with single-threaded event loop like official Redis for better scalability
- **More data structures** - support Lists (`LPUSH`/`RPUSH`/`LRANGE`), Sets (`SADD`/`SMEMBERS`), Hashes (`HSET`/`HGET`), Sorted Sets (`ZADD`/`ZRANGE`)
- **Async logger** - background writer thread with lock-free queue to eliminate logging overhead without stripping logs
- **Connection pooling** - limit max concurrent clients instead of unbounded thread spawning
- **Config file** - load configs from a file instead of compile-time constants