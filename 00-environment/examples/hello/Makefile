BUILD_DIR ?= build
BUILD_TYPE ?= Debug

FORMAT                += app/src/main.cpp
FORMAT                += hal/inc/hal.hpp
FORMAT                += hal/uart/inc/uart.hpp
FORMAT                += hal/uart/inc/uart_stm32.hpp
FORMAT                += hal/uart/src/uart_stm32.cpp

TIDY                  += app/src/main.cpp
TIDY                  += hal/uart/src/uart_stm32.cpp

all: debug

debug:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build $(BUILD_DIR)

release:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)

run: debug
	cmake --build $(BUILD_DIR) --target run_in_renode

format:
	clang-format -i $(FORMAT)

format-check:
	clang-format --dry-run --Werror $(FORMAT)

tidy:
	clang-tidy -p $(BUILD_DIR) $(TIDY)

clean:
	rm -rf $(BUILD_DIR)
