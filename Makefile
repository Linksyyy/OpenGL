CXX = clang++

INCLUDES = -I$(HOME)/.lib/glfw-3.4/include -I$(HOME)/.lib/glad/include -Iincludes

LDFLAGS = -L$(HOME)/.lib/glfw-3.4/build/src -Wl,-rpath,$(HOME)/.lib/glfw-3.4/build/src

LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

TARGET = build/out

SOURCES = main.cpp Shader.cpp stb_image.cpp

GLAD_SRC = $(HOME)/.lib/glad/src/glad.c
GLAD_OBJ = build/glad.o

OBJECTS = $(addprefix build/, $(SOURCES:.cpp=.o))

all: $(TARGET)

$(TARGET): $(OBJECTS) $(GLAD_OBJ)
	$(CXX) $(OBJECTS) $(GLAD_OBJ) -o $(TARGET) $(LDFLAGS) $(LIBS)

$(GLAD_OBJ): $(GLAD_SRC)
	gcc -I$(HOME)/.lib/glad/include -c $(GLAD_SRC) -o $(GLAD_OBJ)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(GLAD_OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
