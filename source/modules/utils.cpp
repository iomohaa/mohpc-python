#include <pymodule.h>

#include <MOHPC/Script/str.h>
#include <MOHPC/Utilities/PropertyMap.h>
#include <MOHPC/Utilities/Info.h>

namespace mohpcpy
{
	class PropertyMapIterator
	{
	private:
		MOHPC::PropertyMapIterator it;
		const MOHPC::PropertyObject* obj;

	public:
		PropertyMapIterator()
			: obj(nullptr)
		{}

		PropertyMapIterator(const MOHPC::PropertyObject* in_obj)
			: obj(in_obj)
		{
		}

		~PropertyMapIterator()
		{
		}

		PropertyMapIterator(PropertyMapIterator&& other) = default;
		PropertyMapIterator& operator=(PropertyMapIterator&& other) = default;
		PropertyMapIterator(const PropertyMapIterator& other) = delete;
		PropertyMapIterator& operator=(const PropertyMapIterator& other) = delete;

		PropertyMapIterator& __iter__()
		{
			return *this;
		}

		PropertyMapIterator& __next__()
		{
			if (!it.getMap())
			{
				it = obj->GetIterator();
				if(!it) throw py::stop_iteration();
				return *this;
			}

			++it;
			if(!it) {
				throw py::stop_iteration();
			}

			return *this;
		}

		const MOHPC::PropertyDef& key() const
		{
			return it.key();
		}

		const MOHPC::str& value() const
		{
			return it.value();
		}
	};

	class PropertyObject
	{
	public:
		static const char* __getattribute__(const MOHPC::PropertyObject* This, const char* name)
		{
			return This->GetPropertyRawValue(name);
		}

		static PropertyMapIterator __iter__(const MOHPC::PropertyObject* This)
		{
			return PropertyMapIterator(This);
		}
	};

	class Vector
	{
	public:
		static py::str __str__(const MOHPC::Vector& This)
		{
			return py::str("(" + MOHPC::str(This.x) + " " + MOHPC::str(This.y) + " " + MOHPC::str(This.z) + ")");
		}
	};
}

SUBMODULE(utils, m)
{
	py::class_<MOHPC::str>(m, "str")
		.def("__str__", &MOHPC::str::c_str);

	py::class_<MOHPC::Vector>(m, "Vector")
		.def("__str__", &mohpcpy::Vector::__str__)
		.def_readwrite("x", &MOHPC::Vector::x)
		.def_readwrite("y", &MOHPC::Vector::y)
		.def_readwrite("z", &MOHPC::Vector::z);

	py::class_<MOHPC::PropertyDef>(m, "ProperyDef")
		.def("__str__", &MOHPC::PropertyDef::GetPropertyName)
		.def_property_readonly("name", &MOHPC::PropertyDef::GetFullPropertyName);

	py::class_<mohpcpy::PropertyMapIterator>(m, "PropertyMapIterator")
		.def("__iter__", &mohpcpy::PropertyMapIterator::__iter__)
		.def("__next__", &mohpcpy::PropertyMapIterator::__next__)
		.def_property_readonly("key", &mohpcpy::PropertyMapIterator::key)
		.def_property_readonly("value", &mohpcpy::PropertyMapIterator::value);

	py::class_<MOHPC::PropertyObject>(m, "PropertyObject")
		.def("__getattribute__", &mohpcpy::PropertyObject::__getattribute__)
		.def("__iter__", &mohpcpy::PropertyObject::__iter__)
		.def("__bool__", &MOHPC::PropertyObject::hasAnyProperty);

	py::class_<MOHPC::ReadOnlyInfo>(m, "ReadOnlyInfo")
		.def("__getattribute__", (MOHPC::str (MOHPC::ReadOnlyInfo::*)(const char*) const)&MOHPC::ReadOnlyInfo::ValueForKey);
}
