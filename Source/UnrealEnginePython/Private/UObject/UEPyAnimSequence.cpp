#include "UnrealEnginePythonPrivatePCH.h"
#include "Animation/AnimSequence.h"
#include "Animation/BlendSpaceBase.h"

PyObject *py_ue_anim_get_skeleton(ue_PyUObject * self, PyObject * args) {
	ue_py_check(self);

	UAnimationAsset *anim = ue_py_check_type<UAnimationAsset>(self);
	if (!anim)
		return PyErr_Format(PyExc_Exception, "UObject is not a UAnimationAsset.");

	USkeleton *skeleton = anim->GetSkeleton();
	if (!skeleton) {
		Py_RETURN_NONE;
	}

	ue_PyUObject *ret = ue_get_python_wrapper((UObject *)skeleton);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}


#if WITH_EDITOR
PyObject *py_ue_anim_sequence_get_raw_animation_data(ue_PyUObject * self, PyObject * args) {
	ue_py_check(self);

	UAnimSequence *anim_seq = ue_py_check_type<UAnimSequence>(self);
	if (!anim_seq)
		return PyErr_Format(PyExc_Exception, "UObject is not a UAnimSequence.");

	PyObject *py_list = PyList_New(0);

	for (FRawAnimSequenceTrack rast : anim_seq->GetRawAnimationData()) {
		PyObject *py_item = py_ue_new_fraw_anim_sequence_track(rast);
		PyList_Append(py_list, py_item);
		Py_DECREF(py_item);
	}

	return py_list;
}

PyObject *py_ue_anim_sequence_get_raw_animation_track(ue_PyUObject * self, PyObject * args) {
	ue_py_check(self);

	int index;
	if (!PyArg_ParseTuple(args, "i:get_raw_animation_track", &index))
		return nullptr;

	UAnimSequence *anim_seq = ue_py_check_type<UAnimSequence>(self);
	if (!anim_seq)
		return PyErr_Format(PyExc_Exception, "UObject is not a UAnimSequence.");

	if (index < 0 || index >= anim_seq->GetAnimationTrackNames().Num())
		return PyErr_Format(PyExc_Exception, "invalid track index %d", index);

	return py_ue_new_fraw_anim_sequence_track(anim_seq->GetRawAnimationTrack(index));
}

PyObject *py_ue_anim_sequence_add_new_raw_track(ue_PyUObject * self, PyObject * args) {
	ue_py_check(self);

	char *name;
	PyObject *py_rast = nullptr;
	if (!PyArg_ParseTuple(args, "s|O:add_new_raw_track", &name, &py_rast))
		return nullptr;

	UAnimSequence *anim_seq = ue_py_check_type<UAnimSequence>(self);
	if (!anim_seq)
		return PyErr_Format(PyExc_Exception, "UObject is not a UAnimSequence.");

	FRawAnimSequenceTrack *rast = nullptr;

	if (py_rast) {
		ue_PyFRawAnimSequenceTrack *py_f_rast = py_ue_is_fraw_anim_sequence_track(py_rast);
		if (py_f_rast) {
			rast = &py_f_rast->raw_anim_sequence_track;
		}
		else {
			return PyErr_Format(PyExc_Exception, "argument is not a FRawAnimSequenceTrack.");
		}
	}

	anim_seq->Modify();

	int32 index = anim_seq->AddNewRawTrack(FName(UTF8_TO_TCHAR(name)), rast);

	if (anim_seq->DoesNeedRebake())
		anim_seq->BakeTrackCurvesToRawAnimation();

	anim_seq->MarkRawDataAsModified();

	return PyLong_FromLong(index);
}
#endif

PyObject *py_ue_anim_set_skeleton(ue_PyUObject * self, PyObject * args) {
	ue_py_check(self);

	PyObject *py_skeleton;
	if (!PyArg_ParseTuple(args, "O:anim_set_skeleton", &py_skeleton))
		return nullptr;

	UAnimationAsset *anim = ue_py_check_type<UAnimationAsset>(self);
	if (!anim)
		return PyErr_Format(PyExc_Exception, "UObject is not a UAnimationAsset.");

	USkeleton *skeleton = ue_py_check_type<USkeleton>(py_skeleton);
	if (!skeleton)
		return PyErr_Format(PyExc_Exception, "argument is not a USkeleton.");

	anim->SetSkeleton(skeleton);
	
	Py_RETURN_NONE;
}

PyObject *py_ue_add_anim_composite_section(ue_PyUObject * self, PyObject * args) {
	ue_py_check(self);

	char *name;
	float time;
	if (!PyArg_ParseTuple(args, "sf:add_anim_composite_section", &name, &time))
		return nullptr;

	UAnimMontage *anim = ue_py_check_type<UAnimMontage>(self);
	if (!anim)
		return PyErr_Format(PyExc_Exception, "UObject is not a UAnimMontage.");

	return PyLong_FromLong(anim->AddAnimCompositeSection(FName(UTF8_TO_TCHAR(name)), time));
}
