#ifndef __OPT_J_H__
#define __OPT_J_H__

typedef struct
{
    double a;
    double b;
    double g;
    double pi;
    double fptildemin;
    double gC;
    double aC;
    double aX;
    double gX;
    double saC;
    double saX;
    double sbC;
    double sbX;
    double S_sub_k;
} jp_struct;

typedef struct
{
    double fpt;
    double alpha;
    double gamma;
    double sigma_a;
    double sigma_b;
    double S_sub_kp;
} fptilde_struct;

#ifdef __cplusplus
extern "C"
{
#endif
    void init_jp(jp_struct *j);
    fptilde_struct *init_fpt(fptilde_struct *fpt_struct, jp_struct *j, double fptilde);
    double func_jp(double f, double fp, fptilde_struct *fpt);
    void run_opt_j();
#ifdef __cplusplus
}
#endif

#endif /* __OPT_J_H__ */