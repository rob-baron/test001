

#include "test_j.h"
#include "opt_j.h"

#include <limits.h>
#include <math.h>
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
    TEST(Optimization, Correctness)
    {
        // GTEST_SKIP() << "Skipping the long correctness test";
        jp_struct j;
        fptilde_struct fpt_min, fpt_dep, *fpt;
        double S_old, S_new, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        init_fpt(&fpt_min, &j, j.fptildemin);
        for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 1)
        {
            fptilde = fptilde_int / 100;
            fpt = &fpt_min;
            if (j.fptildemin < fptilde)
                fpt = init_fpt(&fpt_dep, &j, fptilde);

            for (f_int = -500; f_int <= 500; f_int += 1)
            {
                if (f_int != 0)
                {
                    f = f_int / 100.0;
                    for (fp_int = 0; fp_int <= 1000; fp_int += 1)
                    {
                        fp = fp_int / 100.0;

                        S_old = function_j(f, fp, fptilde);
                        S_new = func_jp(f, fp, fpt);
                        double diff = fabs(S_old - S_new);
                        double abs_S_old, abs_S_new, err;
                        abs_S_old = fabs(S_old);
                        abs_S_new = fabs(S_new);
                        err = MAX(abs_S_old, abs_S_new) * 1E-13;
                        EXPECT_LE(diff, err)
                            << "f=" << f << " f_int=" << f_int << std::endl
                            << "fp=" << fp << " fp_int=" << fp_int << std::endl
                            << "fptilde=" << fptilde << " fptilde_int=" << fptilde_int << std::endl
                            << "diff= " << diff << std::endl
                            << "abs S_old = " << abs_S_old << std::endl
                            << "abs S_new = " << abs_S_new << std::endl
                            << "max_err = " << err << std::endl;
                    }
                }
            }
        }
    }

    /* Optimization group,  correctness test */
    TEST(Optimization, DISABLED_ShortCorrectness)
    {
        jp_struct j;
        fptilde_struct fpt_min, fpt_dep, *fpt;
        double S_old, S_new, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        init_fpt(&fpt_min, &j, j.fptildemin);
        for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 100)
        {
            fptilde = fptilde_int / 100;
            fpt = &fpt_min;
            if (j.fptildemin < fptilde)
                fpt = init_fpt(&fpt_dep, &j, fptilde);

            for (f_int = -500; f_int <= 500; f_int += 100)
            {
                if (f_int != 0)
                {
                    f = f_int / 100.0;
                    for (fp_int = 0; fp_int <= 1000; fp_int += 100)
                    {
                        fp = fp_int / 100.0;

                        S_old = function_j(f, fp, fptilde);
                        S_new = func_jp(f, fp, fpt);
                        EXPECT_DOUBLE_EQ(S_old, S_new)
                            << "f=" << f << " f_int=" << f_int << std::endl
                            << "fp=" << fp << " fp_int=" << fp_int << std::endl
                            << "fptilde=" << fptilde << " fptilde_int=" << fptilde_int << std::endl;
                    }
                }
            }
        }
    }

    TEST(Performance, DISABLED_Original)
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
        fptilde_struct fpt_min, fpt_dep, *fpt;
        double S, f, fp, fptilde;
        int f_int, fp_int, fptilde_int;

        init_jp(&j);
        init_fpt(&fpt_min, &j, j.fptildemin);
        for (fptilde_int = 0; fptilde_int <= 1000; fptilde_int += 1)
        {
            fptilde = fptilde_int / 100;
            fpt = &fpt_min;
            if (j.fptildemin < fptilde)
                fpt = init_fpt(&fpt_dep, &j, fptilde);

            for (f_int = -500; f_int <= 500; f_int += 1)
            {
                if (f_int != 0)
                {
                    f = f_int / 100.0;
                    for (fp_int = 0; fp_int <= 1000; fp_int += 1)
                    {
                        fp = fp_int / 100.0;
                        S = func_jp(f, fp, fpt);
                    }
                }
            }
        }
    }

} // namespace
