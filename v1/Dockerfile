FROM gcc:14 AS builder
WORKDIR /app
COPY Makefile ./
COPY include/ include/
COPY src/ src/
RUN make

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=builder /app/redis-server .
EXPOSE 6379
CMD ["./redis-server"]
