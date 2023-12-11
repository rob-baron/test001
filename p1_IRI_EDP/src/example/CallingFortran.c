/* example to call fortran */
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void t001_();
void t002_(int *);
void t003_(int *);

void main()
{
    /* use gcc to do the dynamic linking */
    void (*funcptr_t001)();
    int ex01[10] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1};
    int ex02[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    long ec;

    int x = 0;
    int y = 1;
    t001_();
    t002_(&x);
    t002_(&y);
    t003_(ex01);
    t003_(ex02);

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
    funcptr_t001();
    printf("got here\n");
    if (ec = dlclose(dl_handle))
    {
        printf("dlerror, %ld", ec);
        printf("%s\n", dlerror());
    }
}