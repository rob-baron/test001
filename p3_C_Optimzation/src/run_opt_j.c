#include "opt_j.h"

void run_opt_j()
{
    jp_struct j;
    double S, f, fp, fptilde;

    init_jp(&j);
    for (f = -5.; f <= 5.; f += 0.01)
    {
        for (fp = 0.; fp <= 10.; fp += 0.01)
        {
            for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01)
            {
                S = func_jp(&j, f, fp, fptilde);
            }
        }
    }
}