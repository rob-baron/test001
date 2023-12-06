#include <stdio.h>
#include <math.h>
#include "opt_j.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

void init_jp(jp_struct *j)
{
    j->a = 0.0081;
    j->b = 0.6;
    j->g = 9.807;
    j->pi = 4. * atan(1.);

    j->fptildemin = (1.0 / 2.0 / j->pi) * pow((4.0 * (j->b) / 5.0), (1.0 / 4.0));

    j->gC = 5.87;
    j->aC = 0.0317;

    j->aX = (log(j->a) - log(j->aC)) / log(j->fptildemin);
    j->gX = -log(j->gC) / log(j->fptildemin);

    j->saC = 0.0547;
    j->saX = 0.32;

    j->sbC = 0.0783;
    j->sbX = 0.16;

    j->S_sub_k = pow(j->g, 2) * pow((2 * j->pi), -4);
}

fptilde_struct *init_fpt(fptilde_struct *fpt_struct, jp_struct *j, double fptilde)
{
    // fpt_struct->fpt = MAX(fptilde, j->fptildemin);
    fpt_struct->fpt = fptilde;

    fpt_struct->alpha = j->aC * pow(fpt_struct->fpt, j->aX);
    fpt_struct->gamma = j->gC * pow(fpt_struct->fpt, j->gX);
    fpt_struct->sigma_a = j->saC * pow(fpt_struct->fpt, j->saX);
    fpt_struct->sigma_b = j->sbC * pow(fpt_struct->fpt, j->sbX);
    return fpt_struct;
}

double func_jp(jp_struct *j, double f, double fp, fptilde_struct *fpt)
{
    double exp1arg = -1.25 * pow((f / fp), -4);
    double sigma = (f <= fp) ? (fpt->sigma_a) : (fpt->sigma_b);
    double exp2arg = -0.5 * pow((f - fp) / (sigma * fp), 2);
    double S = fpt->alpha * (j->S_sub_k) * pow(f, -5) * exp(exp1arg) * pow(fpt->gamma, exp(exp2arg));
    return S;
}
