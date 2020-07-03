#include "pymodule.h"

using namespace mohpcpy;

PYBIND11_MODULE(MOHPCPy, m) {
	m.doc() = "MOHPC Python extension";

	for (submodule* s = submodule::head; s; s = s->next)
	{
		py::module subm = m.def_submodule(s->name);
		// init module
		s->init(subm);
	}
}

submodule* submodule::head = nullptr;

mohpcpy::submodule::submodule(const char* inName, initFunction inInit)
	: name(inName)
	, init(inInit)
{
	next = head;
	head = this;
}
