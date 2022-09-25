CPATH=./C/
TESTPATH=./tests/
M="default push from makefile message"

all: run test

run:
	$(MAKE) -C $(CPATH)

test:
	$(MAKE) -C $(TESTPATH)

run_tests: test
	$(TESTPATH)main_test

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
