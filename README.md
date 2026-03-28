# Redis from Scratch (C++)

A minimal Redis-like in-memory key-value store built from scratch in C++.

## Supported Commands

| Command | Usage |
|---------|-------|
| PING | `PING` |
| SET | `SET key value` |
| GET | `GET key` |
| DEL | `DEL key` |
| QUIT | `QUIT` |

## Version 1

Thread-per-client TCP server with in-memory store.
**Features:** SET/GET/DEL, TTL for cache expiry, file logging.

### Build & Run

```bash
cd v1
make
./redis-server.out
```

### Docker

```bash
cd v1
docker build -t redis-v1 .
docker run -p 6379:6379 redis-v1
```

## Connect

```bash
nc localhost 6379
```