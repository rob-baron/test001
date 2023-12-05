# C Optimization

## Initial Review

The simplest strategy to follow is to optimize the inner loop.  Before even running the code, I reviewed it to get a sense of what is doing.  Since the "function_j" does work on constants, it can easily be improved by moving all of the calculations that involve constants into an initialization routine that sets a structure with those constants to the formulation of the constansts.  Granted the results of this calculation could be done before run time and entered into a configuration file, but as these calculation don't take much time, it is more flexible if it is done in the code.

I am also wondering if all of the logarithms and expodentails are necessary.  I am assuming that they are, but that is another avenue.  As my one professor stated, derive before compute - it can save time.

## created initialization function

This just runs all of the calculations that depend on constansts.  No need to run them multiple times.

## All code was added to a docker file
### To build:
Docker build -f Dockerfile -t p3-c-opt .

### To run within the cloned repo (and to be able to edit/modify the code)
p3_C_Optimzation % Docker run --rm -it --entrypoint bash -v `pwd`:/App/src  p3-c-opt

### To run the google tests in a non interactive mode:
Docker run --rm p3-c-opt

## Created 3 routines that can be tested via google test
### run_func_j

This is just the original main function, but as a function.  This way it can be called from google test as a performance test.

### run_opt_j

This is the modifications to main function to run the optimized function_j code.

### run_correctness

This is to test the correction of the original function j with the optimized function j.  If they don't give the same results than

### Using google test to check correctness
## Code snippet
``` C
    TEST(Optimization, Correctness)
        {
            jp_struct j;
            double S_old, S_new, f, fp, fptilde;

            init_jp(&j);
            for (f = -5.; f <= 5.; f += 0.01)
            {
                for (fp = 0.; fp <= 10.; fp += 0.01)
                {
                    for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01)
                    {
                        S_old = function_j(f, fp, fptilde);
                        S_new = func_jp(&j, f, fp, fptilde);
                        EXPECT_DOUBLE_EQ(S_old, S_new);
                    }
                }
            }
        }

    /* Optimization group,  correctness test */
    TEST(Optimization, ShortCorrectness)
    {
        jp_struct j;
        double S_old, S_new, f, fp, fptilde;

        init_jp(&j);
        for (f = -5.; f <= 5.; f += 1)
        {
            for (fp = 0.; fp <= 10.; fp += 1)
            {
                for (fptilde = 0.; fptilde <= 10.; fptilde += 1)
                {
                    S_old = function_j(f, fp, fptilde);
                    S_new = func_jp(&j, f, fp, fptilde);
                    EXPECT_DOUBLE_EQ(S_old, S_new)
                        << "f=" << f << std::endl
                        << "fp=" << fp << std::endl
                        << "fptilde=" << fptilde << std::endl;
                }
            }
        }
    }
```
Interestingly:

```
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from Optimization
[ RUN      ] Optimization.Correctness
[       OK ] Optimization.Correctness (223038 ms)
[ RUN      ] Optimization.ShortCorrectness
test/test_corr.cpp:47: Failure
Expected equality of these values:
  S_old
    Which is: nan
  S_new
    Which is: nan
f=0
fp=0
fptilde=0
...
test/test_corr.cpp:47: Failure
Expected equality of these values:
  S_old
    Which is: nan
  S_new
    Which is: nan
f=0
fp=10
fptilde=10
[  FAILED  ] Optimization.ShortCorrectness (1 ms)
[----------] 2 tests from Optimization (223039 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (223039 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] Optimization.ShortCorrectness

 1 FAILED TEST
```

What this means is the the function being calculated is ill defined for certain values of f, fp, and fptilde.  It is something to point out to the person who was responsible for deriving it in the first place as they may have made a mistake.  I sent and email asking for clarification.

## Some optimizations
by doing the obvious precalculation of constants, this here is the current time as per gtest:
```
[==========] Running 5 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 1 test from Optimization
[ DISABLED ] Optimization.DISABLED_Correctness
[ RUN      ] Optimization.ShortCorrectness
[       OK ] Optimization.ShortCorrectness (0 ms)
[----------] 1 test from Optimization (0 ms total)

[----------] 4 tests from Performance
[ RUN      ] Performance.Original
[       OK ] Performance.Original (102025 ms)
[ RUN      ] Performance.Optimized
[       OK ] Performance.Optimized (72601 ms)
[ RUN      ] Performance.ShortOriginal
[       OK ] Performance.ShortOriginal (0 ms)
[ RUN      ] Performance.ShortOptimized
[       OK ] Performance.ShortOptimized (0 ms)
[----------] 4 tests from Performance (174626 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 2 test suites ran. (174627 ms total)
[  PASSED  ] 5 tests.
```
Unfortunately the short performance tests are too short, so they will be eliminated.