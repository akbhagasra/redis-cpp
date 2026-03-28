FROM gcc:14 AS builder
WORKDIR /app
COPY Makefile ./
COPY include/ include/
COPY src/ src/
RUN make
CMD ["./redis-server.out"]