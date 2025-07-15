CXX = g++
CXXFLAGS = -g -Wall -I.
CXXIGNORE = -Wno-missing-braces -Wno-format -Wno-sign-compare
CXXFLAGS += $(CXXIGNORE)
APPNAME = main
LIBS = -L./lib

ifeq ($(OS), Windows_NT)	# if the OS is windows add this build condition
	CXXFLAGS += -mwin32
	LIBS +=  -lglew32 -lopengl32 -lglfw3dll
else
	LIBS += -lm -lGL -lGLEW -lglfw3dll -ldl
endif

SRCFILES := $(wildcard src/*.cpp) main.cpp $(wildcard src/Rendering/*.cpp)
OBJS := $(addprefix obj/, $(patsubst %.cpp, %.o, $(SRCFILES)))

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(addprefix obj/, $(notdir $@)) $^

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(addprefix obj/, $(notdir $^)) -o $(APPNAME) $(LIBS)

clean:
	rm obj/*.o
