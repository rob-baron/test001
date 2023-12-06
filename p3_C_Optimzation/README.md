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

This failure is due to the pow(f,-5) when f=0 being taken in the function "function_j", with the line:

``` C
  double S = alpha * pow(g, 2) * pow((2 * pi), -4) * pow(f, -5) * exp(exp1arg) * pow(gamma, exp(exp2arg));
```
I don't see an easy way around this except by not calculating S when f=0.  It is something to explore a bit more, and perhaps bring up with the person who derived the equations being implemented or other expert in the field.

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

## Some more optimization
### upon further inspection of the func_j (optimization of function_j)
There are a set of calculated variables that are only depended on the fptilde fucntion parameter.

Initially, the thought was to move these off into an array, but just as fast and less memory intensive is to reorder the loops so that fptilde is the outer loop.  An additional structure can hold the varialbes that are dependent on fptilde

Made those modifications, however calculating the constant part of:


Caused a slightly different round off such that the EXCPECT_DOUBLE_EQ failed for 1 value when running the longer correctness test.
```
Running main() from /App/cmake-3.27.9/googletest/googletest/src/gtest_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from Optimization
[ RUN      ] Optimization.Correctness
test/test_p3.cpp:60: Failure
Expected equality of these values:
  S_old
    Which is: 5.9894392977973194e-05
  S_new
    Which is: 5.989439297797316e-05
f=4.54 f_int=454
fp=4.5499999999999998 fp_int=455
fptilde=10 fptilde_int=1000
diff= 3.3881317890172014e-20
abs S_old = 5.9894392977973194e-05
abs S_new = 5.989439297797316e-05
max_err = 5.9894392977973199e-18


[  FAILED  ] Optimization.Correctness (211967 ms)
[ RUN      ] Optimization.ShortCorrectness
[       OK ] Optimization.ShortCorrectness (0 ms)
[----------] 2 tests from Optimization (211967 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (211967 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] Optimization.Correctness

 1 FAILED TEST
  YOU HAVE 2 DISABLED TESTS
```

So just reorganizing the loops gives the following performance:
```
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from Performance
[ DISABLED ] Performance.DISABLED_Original
[ RUN      ] Performance.Optimized
[       OK ] Performance.Optimized (50182 ms)
[----------] 1 test from Performance (50182 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (50182 ms total)
[  PASSED  ] 1 test.

  YOU HAVE 3 DISABLED TESTS
  ```

Calculating precalculating the ``` C pow(j->g, 2) * pow((2 * j->pi), -4) ``` of the following:

``` C
    double S = fpt->alpha * pow(j->g, 2) * pow((2 * j->pi), -4) * pow(f, -5) * exp(exp1arg) * pow(fpt->gamma, exp(exp2arg));
```

Yeilds:

```
Running main() from /App/cmake-3.27.9/googletest/googletest/src/gtest_main.cc
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from Performance
[ DISABLED ] Performance.DISABLED_Original
[ RUN      ] Performance.Optimized
[       OK ] Performance.Optimized (35390 ms)
[----------] 1 test from Performance (35390 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (35390 ms total)
[  PASSED  ] 1 test.

  YOU HAVE 3 DISABLED TESTS
```

Honestly this was more than I expected for this small of change

The next part of this pass, I considered changing the following line:
``` C
    double sigma = (f <= fp) * fpt->sigma_a + (f > fp) * fpt->sigma_b;
```
To:
``` C
    double sigma = (f <= fp) ? (fpt->sigma_a) : (fpt->sigma_b);
```

After making multip runs of both of these lines, I currently cannot tell which one is faster.  I am going to go with the second version as it only requires 1 comparison and 1 assignment, where as the original version requires 2 comparisions an addition and an assignmentment.

And finally, noticing the following line in the ```C fptilde_struct *init_fpt(fptilde_struct *fpt_struct, jp_struct *j, double fptilde)```:
``` C
    fpt_struct->fpt = MAX(fptilde, j->fptildemin);
```
As fptildemin is a constanct, it means that the function called with fptildemin can be precalculated as a set of constants.
```
Running main() from /App/cmake-3.27.9/googletest/googletest/src/gtest_main.cc
[==========] Running 2 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 1 test from Optimization
[ RUN      ] Optimization.Correctness
[       OK ] Optimization.Correctness (175820 ms)
[ DISABLED ] Optimization.DISABLED_ShortCorrectness
[----------] 1 test from Optimization (175820 ms total)

[----------] 1 test from Performance
[ DISABLED ] Performance.DISABLED_Original
[ RUN      ] Performance.Optimized
[       OK ] Performance.Optimized (33893 ms)
[----------] 1 test from Performance (33893 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 2 test suites ran. (209713 ms total)
[  PASSED  ] 2 tests.

  YOU HAVE 2 DISABLED TESTS
```
## The smallness of this example
This example was small enough that it was obvious from inpspection as to which function needed attention - ie, you always optimize the inner loop and move computation outward.

## Further possiblities and conclusion.

There are other things that can be done, for example, I suspect that that many of the constansts have relationships that could potentially simplify the calculation (for example saX = 2 * sbX ), however, with the given code I cannot confirm any of my suspicions.

Also, I suspect the mathmatics could be simplified, however, I don't know exactly what the original equations are and am hesitant to change the basic mathmatics without knowing more context, both from a scientific perspective and where this program fits into the general processing.

I considered caching the parts of the function "func_j" that depend only on (f,fp), that is to perform a look up and calculate if it is not present.  I may dd this, but have to consider how much memory this will use and what the structure will look like.

Without changing the mathmatics of how the computation was done, the runtime was decrease to approximately a third of the original (from 102025 ms to 33893 ms).