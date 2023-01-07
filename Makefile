ENGINE_SRC = $(wildcard engine/*.cpp)
ENGINE_H   = $(wildcard engine/*.h)
ENGINE_OUT = ${ENGINE_SRC:.cpp=.o}

GPP = g++
AR = ar

mlib: ${ENGINE_OUT}
	$(AR) rus engine/mlib.a $^

%.o: %.cpp ${ENGINE_H}
	$(GPP) -I . -O2 -o $@ -c $<

interf:
	$(GPP) -o mathengine interface.cpp engine/mlib.a

all:
	make mlib
	make interf

clean:
	rm -rf $(ENGINE_OUT)