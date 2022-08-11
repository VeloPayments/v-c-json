Velo C JSON Library
===================

This is a simple JSON parsing library written in model-checked C. This library
parses a JSON buffer into a tree of data.

The following JSON types are supported. They are wrapped in the given types. A
generic type of `vcjson_val` is used to represent any type. This `vcjson_val`
type can be decoded into a concrete type.

* Number: Represented as the C `double` type, wrapped in the `vcjson_double`
  type.
* String: Represented as a C character string, wrapped in the `vcjson_string`
  type.
* Boolean: Represented as a C `bool` type, wrapped in the `vcjson_bool` type.
* Array: Represented as a C array of `vcjson_val` values.
* Object: Represented as a `vcjson_object` type, which is a dictionary mapping a
  string key to a `vcjson_val` value.
* Null: Represented as a `vcjson_null` type.

Parsing
-------

A JSON buffer can be parsed using the `vcjson_parse` function. For convenience,
a `vcjson_parse_string` function is provided that decomposes a C string into the
buffer and size arguments required for the former function.

Emitting
--------

A JSON string can be emitted from a `vcjson_val` using the `vcjson_emit`
function. This function creates a string using the given allocator instance,
which can then be output. The pretty-print flag will create a human-readable
JSON string when set to `true` and a compact string when set to `false`.
