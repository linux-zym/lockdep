CXX := g++
CXXFLAGS := -pthread -g -O0
TARGET := deadlock

SRCS := deadlock.cpp lockdep.cpp
OBJS := $(SRCS:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)