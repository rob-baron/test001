

#include "test_j.h"
#include "opt_j.h"

#include <limits.h>
#include "gtest/gtest.h"

namespace
{
    /* Optimization group,  correctness test */
    /* To "skip" the test use GTEST_SKIP(), however
       the test can also be skipeed by disabling it thoug
       using DISABLE_ either on the first or second paramber
       of the TEST macro

       The advantage of using DISABLE_ here is to
    */
    TEST(Optimization, DISABLED_Correctness)
    {
        // GTEST_SKIP() << "Skipping the long correctness test";
        jp_struct j;
        double S_old, S_new, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        for (f_int = -500; f_int <= 500; f_int += 1)
        {
            if (f_int != 0)
            {
                f = f_int / 100.0;
                for (fp_int = 0; fp_int <= 1000; fp_int += 1)
                {
                    fp = fp_int / 100.0;
                    for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 1)
                    {
                        fptilde = fptilde_int / 100;
                        S_old = function_j(f, fp, fptilde);
                        S_new = func_jp(&j, f, fp, fptilde);
                        EXPECT_DOUBLE_EQ(S_old, S_new)
                            << "f=" << f << " f_int=" << f_int << std::endl
                            << "fp=" << fp << " fp_int=" << fp_int << std::endl
                            << "fptilde=" << fptilde << " fptilde_int=" << fptilde_int << std::endl;
                    }
                }
            }
        }
    }

    /* Optimization group,  correctness test */
    TEST(Optimization, ShortCorrectness)
    {
        jp_struct j;
        double S_old, S_new, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        for (f_int = -500; f_int <= 500; f_int += 100)
        {
            if (f_int != 0)
            {
                f = f_int / 100.0;
                for (fp_int = 0; fp_int <= 1000; fp_int += 100)
                {
                    fp = fp_int / 100.0;
                    for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 100)
                    {
                        fptilde = fptilde_int / 100;
                        S_old = function_j(f, fp, fptilde);
                        S_new = func_jp(&j, f, fp, fptilde);
                        EXPECT_DOUBLE_EQ(S_old, S_new)
                            << "f=" << f << " f_int=" << f_int << std::endl
                            << "fp=" << fp << " fp_int=" << fp_int << std::endl
                            << "fptilde=" << fptilde << " fptilde_int=" << fptilde_int << std::endl;
                    }
                }
            }
        }
    }

    TEST(Performance, Original)
    {
        // GTEST_SKIP() << "Skipping the long correctness test";

        double S, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        for (f_int = -500; f_int <= 500; f_int += 1)
        {
            if (f_int != 0)
            {
                f = f_int / 100.0;
                for (fp_int = 0; fp_int <= 1000; fp_int += 1)
                {
                    fp = fp_int / 100.0;
                    for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 1)
                    {
                        fptilde = fptilde_int / 100;
                        S = function_j(f, fp, fptilde);
                    }
                }
            }
        }
    }

    TEST(Performance, Optimized)
    {
        jp_struct j;
        double S, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        for (f_int = -500; f_int <= 500; f_int += 1)
        {
            if (f_int != 0)
            {
                f = f_int / 100.0;
                for (fp_int = 0; fp_int <= 1000; fp_int += 1)
                {
                    fp = fp_int / 100.0;
                    for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 1)
                    {
                        fptilde = fptilde_int / 100;
                        S = func_jp(&j, f, fp, fptilde);
                    }
                }
            }
        }
    }

    TEST(Performance, ShortOriginal)
    {
        jp_struct j;
        double S, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        for (f_int = -500; f_int <= 500; f_int += 100)
        {
            if (f_int != 0)
            {
                f = f_int / 100.0;
                for (fp_int = 0; fp_int <= 1000; fp_int += 100)
                {
                    fp = fp_int / 100.0;
                    for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 100)
                    {
                        fptilde = fptilde_int / 100;
                        S = function_j(f, fp, fptilde);
                    }
                }
            }
        }
    }

    TEST(Performance, ShortOptimized)
    {
        jp_struct j;
        double S, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        for (f_int = -500; f_int <= 500; f_int += 100)
        {
            if (f_int != 0)
            {
                f = f_int / 100.0;
                for (fp_int = 0; fp_int <= 1000; fp_int += 100)
                {
                    fp = fp_int / 100.0;
                    for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 100)
                    {
                        fptilde = fptilde_int / 100;
                        S = func_jp(&j, f, fp, fptilde);
                    }
                }
            }
        }
    }

} // namespace
