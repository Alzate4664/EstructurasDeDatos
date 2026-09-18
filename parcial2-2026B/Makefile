CXX = g++
CXXFLAGS = -Wall -std=c++11
DEPFLAGS = -MMD -MP

TARGET = bd_sql
SRCS = main.cpp ArbolBPlus.cpp IndiceSecundarioBPlus.cpp AnalizadorSQL.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)