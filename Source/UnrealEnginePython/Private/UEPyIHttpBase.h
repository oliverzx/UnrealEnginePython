#pragma once

#include "UnrealEnginePython.h"

#include "Runtime/Online/HTTP/Public/HttpModule.h"

typedef struct {
	PyObject_HEAD
	/* Type-specific fields go here. */
	TSharedRef<IHttpBase> http_base;
} ue_PyIHttpBase;


void ue_python_init_ihttp_base(PyObject *);
