#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace mohpcpy
{
	class submodule
	{
	public:
		using initFunction = void(*)(py::module& m_);

		submodule* next;
		static submodule* head;
		const char* name;
		initFunction init;

	public:
		submodule(const char* inName, initFunction init);
	};

	#define SUBMODULE(name, variable) \
	void init_##name(py::module& variable); \
	mohpcpy::submodule module_##name(#name, init_##name); \
	void init_##name(py::module& variable)
}
