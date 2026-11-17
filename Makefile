BUILD := build

.PHONY: build run clean
build:
	cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD) -j

run: build
	./$(BUILD)/solar-sym

clean:
	rm -rf $(BUILD)
