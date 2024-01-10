/* example to call fortran
  # f77 -shared -fPIC -ofortranSub.so fortranSub.for
  # gcc -Wall CallingFortran.c /App/test001/p1_IRI_EDP/src/example/FortranSub.so
  # ./a.out
*/
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void t001_();
void t002_(int *);
void t003_(int *);
void t004_(int *, long *);
void t005_(int *, long *, float *, float *);
void t006_(double *, double *);
void t007_(float *);

void main()
{
    /* use gcc to do the dynamic linking */
    void (*funcptr_t001)();
    int ex01[10] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1};
    int ex02[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    float alati = 74.5, along = 10.1;
    double da = 74.15, db = 10.15;
    long ec;
    long jmag;

    int x = 0;
    int y = 1;
    float outf[20 * 1000];
    /*
    t001_();
    t002_(&x);
    t002_(&y);
    t003_(ex01);
    t003_(ex02);

    jmag = 1;
    t004_(ex02, &jmag);
    jmag = -1;
    t004_(ex02, &jmag);

    jmag = 0;
    t004_(ex02, &jmag);

    t005_(ex02, &jmag, &alati, &along);
    t006_(&da, &db);
    */

    t007_(outf);
    for (int xp = 0; xp < 100; xp++)
    {
        int x_index = xp * 20;
        printf(" %d:  ", x_index);
        for (int yp = 0; yp < 20; yp++)
        {
            printf("%f", outf[yp + x_index]);
        }
        printf("\n");
    }
    /* or to dynamically load the shared object */
    void *dl_handle = dlopen("./fortranSub.so", RTLD_LAZY);
    if (!dl_handle)
    {
        printf("> %p %s\n", dl_handle, dlerror());
    }

    funcptr_t001 = (void (*)(void))dlsym(dl_handle, "t001_");

    if (!funcptr_t001)
    {
        printf("? %p %s\n", funcptr_t001, dlerror());
    }
    /* funcptr_t001(); */

    if ((ec = dlclose(dl_handle)) > 0)
    {
        printf("dlerror, %ld", ec);
        printf("%s\n", dlerror());
    }
}
8146435166