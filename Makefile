CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread
TARGET = redis.out
DEBUG_TARGET = redis.debug.out
RELEASE_TARGET = redis.release.out
LOG_FILE = redis.log

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS = -static
else
	LDFLAGS =
endif

SRCS = $(wildcard src/*.cc)

$(TARGET): $(SRCS) include/*.h
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LDFLAGS)

release: $(SRCS) include/*.h
	$(CXX) $(CXXFLAGS) -DRELEASE_MODE -o $(RELEASE_TARGET) $(SRCS) $(LDFLAGS)

debug: $(SRCS) include/*.h
	$(CXX) -std=c++17 -g -O0 -pthread -o $(DEBUG_TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(RELEASE_TARGET) $(LOG_FILE)
	rm -rf $(DEBUG_TARGET)**

run: $(TARGET)
	./$(TARGET)
