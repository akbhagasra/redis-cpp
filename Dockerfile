FROM gcc:14 AS builder
WORKDIR /app
COPY Makefile ./
COPY include/ include/
COPY src/ src/
RUN make release

FROM alpine:3.20
WORKDIR /app
COPY --from=builder /app/redis.release.out .
CMD ["./redis.release.out"]