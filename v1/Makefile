CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread
LDFLAGS = -static-libstdc++ -static-libgcc
TARGET = redis-server.out

SRCS = $(wildcard src/*.cc)

$(TARGET): $(SRCS) include/*.h
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
