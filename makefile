CPATH=./C/
TESTPATH=./test/
M="default push from makefile message"

all: run tests

run:
	$(MAKE) -C $(CPATH)

tests:
	$(MAKE) -C $(TESTPATH)

run_tests: tests
	$(TESTPATH)vector_test

push: clean
	git add .
	git commit -m "$(M)"
	git push

.PHONY: all clean

clean:
	$(MAKE) clean -C $(CPATH)
	$(MAKE) clean -C $(TESTPATH)
