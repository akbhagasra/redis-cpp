CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread
TARGET = redis-server.out

SRCS = $(wildcard src/*.cc)

$(TARGET): $(SRCS) include/*.h
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
