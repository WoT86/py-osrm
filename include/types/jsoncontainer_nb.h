#ifndef OSRM_NB_JSONCONTAINER_H
#define OSRM_NB_JSONCONTAINER_H

#include "util/json_container.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/unique_ptr.h>

#include <nanobind/stl/string.h>
#include <nanobind/stl/variant.h>


void init_JSONContainer(nanobind::module_& m);

namespace json = osrm::util::json;
using JSONValue = std::variant<json::String,
                                json::Number,
                                json::Object,
                                json::Array,
                                json::True,
                                json::False,
                                json::Null>;
//Custom Type Casters
namespace nanobind::detail {

template <> struct type_caster<JSONValue> : type_caster_base<JSONValue> {
    template <typename T> using Caster = make_caster<detail::intrinsic_t<T>>;

    template <typename T>
    static handle from_cpp(T&& val, rv_policy policy, cleanup_list* cleanup) noexcept {
        return std::visit([&](auto&& v) {
            return Caster<decltype(v)>::from_cpp(std::forward<decltype(v)>(v), policy, cleanup);
        }, std::forward<T>(val));
    }
};

template <> struct type_caster<json::String> : type_caster_base<json::String> {
    static handle from_cpp(const json::String& val, rv_policy, cleanup_list*) noexcept {
        return PyUnicode_FromStringAndSize(val.value.c_str(), val.value.size());
    }
};
template <> struct type_caster<json::Number> : type_caster_base<json::Number> {
    static handle from_cpp(const json::Number& val, rv_policy, cleanup_list*) noexcept {
        return PyFloat_FromDouble((double)val.value);
    }
};

template <> struct type_caster<json::True> : type_caster_base<json::True> {
    static handle from_cpp(const json::True&, rv_policy, cleanup_list*) noexcept {
        return handle(Py_True).inc_ref();
    }
};
template <> struct type_caster<json::False> : type_caster_base<json::False> {
    static handle from_cpp(const json::False&, rv_policy, cleanup_list*) noexcept {
        return handle(Py_False).inc_ref();
    }
};
template <> struct type_caster<json::Null> : type_caster_base<json::Null> {
    static handle from_cpp(const json::Null&, rv_policy, cleanup_list*) noexcept {
        return none().release();
    }
};

} //nanobind::detail

struct ValueStringifyVisitor {
    std::string operator()(const json::String& str) {
        return "'" + str.value + "'";
    }
    std::string operator()(const json::Number& num) {
        return std::to_string(num.value);
    }
    std::string operator()(const json::True& str) {
        return "True";
    }
    std::string operator()(const json::False&) {
        return "False";
    }
    std::string operator()(const json::Null&) {
        return "None";
    }

    std::string operator()(const json::Array& arr) {
        std::string output = "[";
        for(int i = 0; i < arr.values.size(); ++i) {
            if(i != 0) {
                output += ", ";
            }
            output += std::visit(*this, arr.values[i]);
        }
        return output + "]";
    }

    std::string operator()(const json::Object& obj) {
        std::string output = "{";
        bool first = true;
        for(auto itr : obj.values) {
            if(!first) {
                output += ", ";
            }
            output += "'";
            output += itr.first;
            output += "': ";
            output += std::visit(*this, itr.second);
            first = false;
        }
        return output + "}";
    }
};

#endif //OSRM_NB_JSONCONTAINER_H
