#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "mymodule.h"
#include "myclass.h"

//#define STATIC_EXTENSION_T

/******************************************************************************/
/* module object definition                                                   */
/******************************************************************************/


int mymodule_exec(PyObject *mod)
{
    PyObject *class = myclassobj_new();
    PyModule_AddObject(mod, "MyClass", class);

    return 0;
}

#ifndef STATIC_EXTENSION_T
static PyModuleDef_Slot mymodule_slots[] = {
    { Py_mod_exec, mymodule_exec },
    { 0, NULL }
};
#endif

static struct PyModuleDef mymodule = {
    PyModuleDef_HEAD_INIT,
    .m_name    = "mymodule",
    .m_doc     = PyDoc_STR("Python example module with one class"),
    .m_size    = 0,
#ifndef STATIC_EXTENSION_T
    .m_slots   = mymodule_slots,
#endif
};

DLLExport PyMODINIT_FUNC PyInit_mymodule(void)
{
    PyObject *mod;

#ifndef STATIC_EXTENSION_T
    mod = PyModuleDef_Init(&mymodule);
#else
    mod = PyModule_Create(&mymodule);
    mymodule_exec(mod);
#endif

    return mod;
}