CPATH=./src/
PYTHONPATH=./python/
TESTPATH=./tests/
BENCHPATH=./benchmarks/
PYTHON=python
NODE=node
M="default push from makefile message"

all: run

# cmd to type: make generate TYPE=oneof('vector', 'matrix', 'tensor') SIZE=integer
generate:
	@echo "Generating data files..."
	@cd $(PYTHONPATH) && $(NODE) object_generator.js $(TYPE) $(SIZE)

run:
	@echo "Running Python main..."
	@cd $(PYTHONPATH) && $(PYTHON) main.py

clib:
	@echo "Running C library main..."
	@cd $(CPATH) && make run

test:
	$(MAKE) -C $(TESTPATH)

run_tests: test
	$(TESTPATH)main_test

bench:
	$(MAKE) -C $(BENCHPATH)

commit: clean
	git add .
	git commit -m $(M)

push: clean
	git add .
	git commit -m $(M)
	git push

.PHONY: all clean

clean:
	$(MAKE) clean -C $(CPATH)
	$(MAKE) clean -C $(TESTPATH)
	$(MAKE) clean -C $(BENCHPATH)
	clear
