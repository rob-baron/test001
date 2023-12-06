#ifndef __TEST_J_H__
#define __TEST_J_H__
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#ifdef __cplusplus
extern "C"
{
#endif
    double function_j(double f, double fp, double fptilde);
    void run_func_j();
#ifdef __cplusplus
}
#endif

#endif /* __TEST_J_H__ */