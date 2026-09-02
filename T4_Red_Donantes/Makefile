CXX = g++
CXXFLAGS = -Wall -std=c++11 -I./Datos -I./Estructuras -I./Visualizacion

TARGET = bin/demo_estructuras

DATOS_SRC = Datos/Person.cpp
ESTR_SRC = Estructuras/LinkedList.cpp Estructuras/DoublyLinkedList.cpp \
           Estructuras/Stack.cpp Estructuras/Queue.cpp Estructuras/BST.cpp \
           Estructuras/HashTable.cpp Estructuras/Graph.cpp Estructuras/BinaryTree.cpp
VIS_SRC = Visualizacion/Visualizer.cpp
MIG_SRC = Migraciones/main.cpp

SRCS = $(DATOS_SRC) $(ESTR_SRC) $(VIS_SRC) $(MIG_SRC)

OBJS = $(patsubst %.cpp,obj/%.o,$(notdir $(SRCS)))

VPATH = Datos Estructuras Visualizacion Migraciones

all: dirs $(TARGET)

dirs:
	@mkdir -p obj bin

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj bin demo_estructuras
