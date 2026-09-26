BUILD_DIR ?= build
BUILD_TYPE ?= Debug

all: debug

debug:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR)

release:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)

run: debug
	cmake --build $(BUILD_DIR) --target run_in_renode

clean:
	rm -rf $(BUILD_DIR)
