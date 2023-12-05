
cc:=gcc -I.
cpp:=g++ -I.

test_j.o: src/test_j.c
	$(cc) -c src/test_j.c

opt_j.o: src/opt_j.c src/opt_j.h
	$(cc) -c src/opt_j.c

run_test_j.o: src/run_test_test_j.c
	$(cc) -c src/run_test_test_j.c

run_opt_j.o: src/run_opt_j.c
	$(cc) -c src/run_opt_j.c

test_p3.o: test/test_p3.cpp src/test_j.h src/opt_j.h
	$(cpp) -g -Wall -Isrc -c test/test_p3.cpp

test_p3: test_j.o opt_j.o test_p3.o
	$(cpp) test_j.o opt_j.o test_p3.o -lm -lgtest -lgtest_main -o test_p3

check: test_p3
	./test_p3 --disable_all

clean:
	rm -rf run_opt_j.o
	rm -rf test_corr.o
	rm -rf test_j.o
	rm -rf test_corr
	rm -rf test_perf