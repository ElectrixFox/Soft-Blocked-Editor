CXX = gcc
CXXFLAGS = -g -Wall -I.
CXXIGNORE = -Wno-missing-braces -Wno-format	-Wno-incompatible-pointer-types
CXXFLAGS += $(CXXIGNORE)
APPNAME = main
LIBS = -L./lib

ifeq ($(OS), Windows_NT)	# if the OS is windows add this build condition
	CXXFLAGS += -mwin32
	LIBS +=  -lglew32 -lopengl32 -lglfw3dll
else
	LIBS += -lm -lGL -lGLEW -lglfw3dll -ldl
endif

SRCFILES := $(wildcard src/*.c) main.c $(wildcard src/Rendering/*.c)
OBJS := $(addprefix obj/, $(patsubst %.c, %.o, $(SRCFILES)))

obj/%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $(addprefix obj/, $(notdir $@)) $^

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(addprefix obj/, $(notdir $^)) -o $(APPNAME) $(LIBS)

clean:
	rm obj/*.o
