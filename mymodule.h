#ifndef _MODULE_H_
#define _MODULE_H_

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(_WIN32)
#define DLLExport __declspec(dllexport)
#define DLLImport __declspec(dllimport)
#else
#define DLLExport
#define DLLImport
#endif /* _WIN32 */

DLLExport PyMODINIT_FUNC PyInit_mymodule(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MODULE_H_ */