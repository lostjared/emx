PROJECT = scroller

# Compiler
EMCC = em++

# Source Files
SRC = main.cpp ../src/camera.cpp ../src/game.cpp ../src/game_level.cpp ../src/gfx_data.cpp ../src/gfx_file.cpp ../src/level.cpp ../src/object.cpp ../src/particles.cpp ../src/star_field.cpp ../src/window.cpp

# Compiler Flags
CXXFLAGS = -O2 -std=c++20 -I../src -DFOR_WASM -s USE_SDL=2 -s USE_SDL_TTF=2

# Emscripten Linker Flags
LDFLAGS =  -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file assets

# Output Files
OUT_HTML = $(PROJECT).html
OUT_JS = $(PROJECT).js
OUT_WASM = $(PROJECT).wasm

# Build Target
all: $(OUT_HTML) $(OUT_JS) $(OUT_WASM)

$(OUT_HTML): $(SRC)
	$(EMCC) $(CXXFLAGS) $(SRC) -o $(OUT_HTML) $(LDFLAGS)

# Clean Target
clean:
	rm -f $(OUT_HTML) $(OUT_JS) $(OUT_WASM)

.PHONY: all clean
