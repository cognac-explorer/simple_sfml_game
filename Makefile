CXX       := em++
OUTPUT    := game

RAYLIB_DIR := external/raylib
RAYLIB_LIB := $(RAYLIB_DIR)/src/libraylib.web.a
RAYLIB_INC := $(RAYLIB_DIR)/src

SHELL_FILE := minimal_template.html

CXX_FLAGS := -O3 -std=c++17 -Wall -Wno-missing-braces -I$(RAYLIB_INC)

LDFLAGS   := -O3 -s USE_GLFW=3 -s ASYNCIFY \
	--preload-file bin/images@images \
	--preload-file bin/arial.ttf@arial.ttf \
	--preload-file bin/assets.txt@assets.txt \
	--preload-file bin/level.txt@level.txt \
	--preload-file bin/player.txt@player.txt \
	--shell-file $(SHELL_FILE) \
	-o ./bin/$(OUTPUT).html

SRC_FILES := $(wildcard src/*.cpp)

OBJ_FILES := $(SRC_FILES:.cpp=.o)

all: $(OUTPUT).html

$(OUTPUT).html: $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(RAYLIB_LIB) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXX_FLAGS) -I./src $< -o $@

clean:
	rm -f $(OBJ_FILES) ./bin/$(OUTPUT).html ./bin/$(OUTPUT).js ./bin/$(OUTPUT).wasm ./bin/$(OUTPUT).data

run: $(OUTPUT).html
	@echo "Serving files on http://localhost:8080/$(OUTPUT).html. Press Ctrl+C to stop."
	python3 -m http.server 8080 -d bin/

.PHONY: all clean run
