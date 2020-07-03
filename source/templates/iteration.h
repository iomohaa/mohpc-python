#pragma once

namespace mohpcpy
{
	/**
	 * Class for iterating through elements.
	 */
	template<
		typename base_class,
		typename item_type
		>
	class ClassIterator
	{
	private:
		using class_type = typename std::remove_reference<base_class>::type;

		using num_f = size_t(class_type::*)() const;
		using getter_f = item_type * (class_type::*)(uintptr_t id) const;

	private:
		base_class obj;
		num_f num_pf;
		getter_f getter_pf;

	public:
		ClassIterator(class_type&& in_obj, num_f in_num_pf, getter_f in_getter_pf)
			: obj(std::move(in_obj))
			, num_pf(in_num_pf)
			, getter_pf(in_getter_pf)
		{
		}

		ClassIterator(class_type& in_obj, num_f in_num_pf, getter_f in_getter_pf)
			: obj(in_obj)
			, num_pf(in_num_pf)
			, getter_pf(in_getter_pf)
		{
		}

		ClassIterator(class_type* in_obj, num_f in_num_pf, getter_f in_getter_pf)
			: ClassIterator(*in_obj, in_num_pf, in_getter_pf)
		{
		}

		size_t __len__() const
		{
			return (obj.*num_pf)();
		}

		template<bool is_pointer>
		item_type& __getitem__(uintptr_t index) const;

		template<>
		item_type& __getitem__<true>(uintptr_t index) const
		{
			const size_t num = (obj.*num_pf)();
			if (index >= num) {
				throw py::index_error();
			}

			return *(obj.*getter_pf)(index);
		}

		template<>
		item_type& __getitem__<false>(uintptr_t index) const
		{
			const size_t num = (obj.*num_pf)();
			if (index >= num) {
				throw py::index_error();
			}

			return (obj.*getter_pf)(index);
		}
	};

	template<typename T>
	T& to_ref(T* ref)
	{
		return *ref;
	}

	template<typename T>
	T& to_ref(T& ref)
	{
		return ref;
	}

	template<typename T>
	T to_ref(T ref)
	{
		return ref;
	}

	#define DECLARE_ITERATOR(name, base_class, item_type, fn_num, fn_get) \
	class name { \
	private: \
		using class_type = typename std::remove_reference<base_class>::type; \
		base_class obj; \
	public: \
		template<typename T> name(T&& in_obj) : obj(std::forward<class_type>(in_obj)){} \
		size_t __len__() const { return obj.fn_num(); } \
		item_type __getitem__(uintptr_t index) const { \
			const size_t num = obj.fn_num(); \
			if (index >= num) { throw py::index_error(); } \
			return to_ref(obj.fn_get(index)); \
		} \
	};

	#define DECLARE_GETTER(name, iterator, type) static iterator name(type* This) { return iterator(*This); }

	/** Declare a class for iteration. */
	template<typename T>
	void declare_iterator(py::module& m, const char* name, const char* doc = "")
	{
		py::class_<T>(m, name, doc)
			.def("__len__", &T::__len__)
			.def("__getitem__", &T::__getitem__, py::return_value_policy::reference_internal);
	}

	template<typename T>
	void declare_iterator(py::object& obj, const char* name, const char* doc = "")
	{
		py::class_<T>(obj, name, doc)
			.def("__len__", &T::__len__)
			.def("__getitem__", &T::__getitem__, py::return_value_policy::reference_internal);
	}
}

