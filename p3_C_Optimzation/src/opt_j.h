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

#ifdef __cplusplus
extern "C"
{
#endif
    void init_jp(jp_struct *j);
    double func_jp(jp_struct *j, double f, double fp, double fptilde);
    void run_opt_j();
#ifdef __cplusplus
}
#endif

#endif /* __OPT_J_H__ */