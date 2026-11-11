CXX      ?= g++
CXXFLAGS ?= -O2 -Wall
SRCS     := $(wildcard *.cpp)

ifeq ($(shell uname),Darwin)
  LIBS := -framework GLUT -framework OpenGL -Wno-deprecated-declarations
else
  LIBS := -lglut -lGLU -lGL
endif

grass: $(SRCS) $(wildcard *.h)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LIBS)

clean:
	rm -f grass

.PHONY: clean
