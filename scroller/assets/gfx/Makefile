

CXX_FLAGS=-std=c++17 -O2 -Wall
SDL_CXXFLAGS= 
SDL_LDFLAGS= 
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.cpp=.o)))
OUTPUT_NAME=gfx-cmp

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(SDL_CXXFLAGS) -c -o $@ $<
  
all: $(OBJ_FILES) 
	$(CXX) $(CXX_FLAGS) -c ../gfx_file.cpp -o gfx_file.o
	$(CXX) $(OBJ_FILES) gfx_file.o  -o $(OUTPUT_NAME)  $(LDFLAGS) $(SDL_LDFLAGS)

install: all
	cp $(OUTPUT_NAME) /usr/local/bin

clean:
	rm -f $(OUTPUT_NAME) *.o 
