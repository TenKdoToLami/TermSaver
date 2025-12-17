.PHONY: all run clean

all:
	@mkdir -p build
	@cd build && cmake .. && make

run: all
	@./build/termsaver

clean:
	@rm -rf build
