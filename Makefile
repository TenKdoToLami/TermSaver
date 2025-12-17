.PHONY: all run clean docs clean-docs

all:
	@mkdir -p build
	@cd build && cmake .. && make

run: all
	@./build/termsaver

clean:
	@rm -rf build docs

docs:
	@doxygen Doxyfile
	@echo "Documentation generated in docs/"

clean-docs:
	@rm -rf docs
