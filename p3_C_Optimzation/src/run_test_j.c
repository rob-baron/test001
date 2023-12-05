#include "test_j.h"

run_test_j()
{
    double S, f, fp, fptilde;

    for (f = -5.; f <= 5.; f += 0.01)
    {
        for (fp = 0.; fp <= 10.; fp += 0.01)
        {
            for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01)
            {
                S = function_j(f, fp, fptilde);
            }
        }
    }
}
