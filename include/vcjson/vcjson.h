/**
 * \file include/vcjson/vcjson.h
 *
 * \brief Main header file for vcjson.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/allocator.h>
#include <rcpr/status.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/**
 * \brief JSON value type.
 */
enum vcjson_value_type
{
    VCJSON_VALUE_TYPE_NULL,
    VCJSON_VALUE_TYPE_OBJECT,
    VCJSON_VALUE_TYPE_ARRAY,
    VCJSON_VALUE_TYPE_NUMBER,
    VCJSON_VALUE_TYPE_STRING,
    VCJSON_VALUE_TYPE_BOOL
};

/**
 * \brief JSON object type.
 */
typedef struct vcjson_object vcjson_object;

/**
 * \brief JSON object iterator.
 */
typedef struct vcjson_object_iterator vcjson_object_iterator;

/**
 * \brief JSON array type.
 */
typedef struct vcjson_array vcjson_array;

/**
 * \brief JSON number type.
 */
typedef struct vcjson_number vcjson_number;

/**
 * \brief JSON string type.
 */
typedef struct vcjson_string vcjson_string;

/**
 * \brief JSON bool type.
 */
typedef struct vcjson_bool vcjson_bool;

/**
 * \brief JSON null type.
 */
typedef struct vcjson_null vcjson_null;

/**
 * \brief Wrapper JSON value type.
 */
typedef struct vcjson_value vcjson_value;

/**
 * \brief the JSON null singleton for this library.
 */
extern const vcjson_null* VCJSON_NULL;

/**
 * \brief the JSON true singleton for this library.
 */
extern const vcjson_bool* VCJSON_TRUE;

/**
 * \brief the JSON false singleton for this library.
 */
extern const vcjson_bool* VCJSON_FALSE;

/* error codes. */
#define ERROR_VCJSON_INVALID_GET                        0x6300
#define ERROR_VCJSON_KEY_NOT_FOUND                      0x6301
#define ERROR_VCJSON_ITERATOR_END                       0x6302
#define ERROR_VCJSON_ITERATOR_BAD                       0x6303
#define ERROR_VCJSON_ARRAY_INDEX_OUT_OF_BOUNDS          0x6304

/**
 * \brief Create an empty \ref vcjson_object using the given allocator.
 *
 * \note On success, this function creates a  \ref vcjson_object instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param obj           Pointer to the object pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_create(vcjson_object** obj, RCPR_SYM(allocator)* alloc);

/**
 * \brief Make a deep copy of the given object.
 *
 * \note On success, this function creates a  \ref vcjson_object instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param obj           Pointer to the object pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original object to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_copy(
    vcjson_object** obj, RCPR_SYM(allocator)* alloc, const vcjson_object* orig);

/**
 * \brief Get the number of elements in the given object instance.
 *
 * \param obj           The array instance for this operation.
 *
 * \returns the number of elements in this object instance.
 */
size_t vcjson_object_elements(const vcjson_object* obj);

/**
 * \brief Put a value into the given \ref vcjson_object instance.
 *
 * \note On success, the \ref vcjson_object assumes ownership of this value. If
 * there is already a value associated with the given key, its resource is
 * released via \ref resource_release.
 *
 * \param obj           The object instance for this operation.
 * \param key           The key for this operation.
 * \param value         The value to insert into this object.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_put(
    vcjson_object* obj, const char* key, vcjson_value* value);

/**
 * \brief Get a value from the object using the given key.
 *
 * On success, the value pointer is updated with the \ref vcjson_value instance
 * associated with the given key in this object.
 *
 * \note Ownership of the returned value remains with the object.
 *
 * \param value         Pointer to the value pointer to be updated on success.
 * \param obj           The object instance for this operation.
 * \param key           The key for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_KEY_NOT_FOUND if the key is not associated with a value.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_get(
    vcjson_value** value, vcjson_object* obj, const char* key);

/**
 * \brief Remove the given key from the object.
 *
 * \note This operation releases the resource relating to any value associated
 * with the given key. This operation will fail with \ref
 * VCJSON_ERROR_KEY_NOT_FOUND if this key is not found in the object. This
 * failure case should be checked if the caller does not know whether the given
 * key exists in the object.
 *
 * \param obj           The object instance for this operation.
 * \param key           The key for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_remove(vcjson_object* obj, const char* key);

/**
 * \brief Clear the given object of all key-value pairs.
 *
 * \note Any key-value pair associated with this object will be released using
 * \ref resource_release. When this operation completes successfully, the object
 * is reset to the same state it was in when first created.
 *
 * \param obj           The object instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_clear(vcjson_object* obj);

/**
 * \brief Create an object iterator set to the first key-value pair in this
 * object instance.
 *
 * On success, the given object iterator pointer is set to the first key-value
 * pair in this object instance.
 *
 * \note This iterator is only valid as long as the object instance is not
 * modified. If the object instance is modified, using this iterator will cause
 * undefined behavior. This iterator is a resource and must be released using
 * \ref resource_release when no longer needed.
 *
 * \param iterator      Pointer to the iterator pointer to be updated with a new
 *                      iterator instance.
 * \param obj           The object instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_ITERATOR_END if there are no key-value pairs in this
 *        object instance.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_create(
    vcjson_object_iterator** iterator, vcjson_object* obj);

/**
 * \brief Increment the given iterator to the next key-value pair in the object
 * to which it points.
 *
 * \note This iterator is only valid as long as the object instance is not
 * modified. If the object instance is modified, using this iterator will cause
 * undefined behavior.
 *
 * \param iterator      The iterator to increment.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_ITERATOR_END if the iterator has been incremented to the
 *        end of the object instance.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_next(vcjson_object_iterator* iterator);

/**
 * \brief Get the key-value pair associated with the current iterator position.
 *
 * \note The key and value are both owned by the object and cannot be modified.
 * Furthermore, these values should not be used if the underlying object is
 * modified, as these pointers will no longer be valid. If the iterator is
 * invalid (an error was returned when it was iterated or created), then it is
 * an error to use this operation and the result is undefined and unsafe.
 *
 * \param key           Pointer to receive the key value at this location.
 * \param value         Pointer to receive the value pointer at this location.
 * \param iterator      The iterator for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_ITERATOR_BAD if the iterator is bad.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_object_iterator_value(
    const char** key, vcjson_value** value, vcjson_object_iterator* iterator);

/**
 * \brief Get the resource handle for the given \ref vcjson_object instance.
 *
 * \param obj           The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_object_resource_handle(vcjson_object* obj);

/**
 * \brief Get the resource handle for the given \ref vcjson_object_iterator
 * instance.
 *
 * \param iterator      The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_object_iterator_resource_handle(
    vcjson_object_iterator* iterator);

/**
 * \brief Create a \ref vcjson_array using the given allocator and size.
 *
 * \note On success, this function creates a  \ref vcjson_array instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle. By default, all values in this array will be initialized to \ref
 * VCJSON_NULL. The caller should use \ref vcjson_array_set and \ref
 * vcjson_array_get to change and get values in this array, respectfully.
 *
 * \param arr           Pointer to the array pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param size          The number of elements in this array.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_create(
    vcjson_array** arr, RCPR_SYM(allocator)* alloc, size_t size);

/**
 * \brief Make a deep copy of the given \ref vcjson_array.
 *
 * \note On success, this function creates a  \ref vcjson_array instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param arr           Pointer to the array pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original array to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_copy(
    vcjson_array** arr, RCPR_SYM(allocator)* alloc, const vcjson_array* orig);

/**
 * \brief Get the size of the given array instance.
 *
 * \param arr           The array instance for this operation.
 *
 * \returns the size of this array instance.
 */
size_t vcjson_array_size(const vcjson_array* arr);

/**
 * \brief Set the value of the \ref vcjson_array instance at the given offset.
 *
 * \note On success, this array instance takes ownership of the given \ref
 * vcjson_value instance. If there is already a value set by the array at the
 * given offset, it is reclaimed using \ref resource_release.
 *
 * \param arr           The array instance for this operation.
 * \param offset        The offset at which this value should be set.
 * \param value         The value to set.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_set(vcjson_array* arr, size_t offset, vcjson_value* value);

/**
 * \brief Get the value of the \ref vcjson_array instance at the given offset.
 *
 * \note On success, the value at the given offset of the array is returned.
 * This value remains owned by this array instance.
 *
 * \param value         Pointer to the value pointer to receive this value. Note
 *                      that this value is owned by the array instance.
 * \param arr           The array instance for this operation.
 * \param offset        The offset for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_array_get(vcjson_value** value, vcjson_array* arr, size_t offset);

/**
 * \brief Get the resource handle for the given \ref vcjson_array instance.
 *
 * \param arr           The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_array_resource_handle(vcjson_array* arr);

/**
 * \brief Create a \ref vcjson_number instance using the given allocator and
 * numeric value.
 *
 * \note On success, this function creates a \ref vcjson_number instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param number        Pointer to the number pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param value         The number value to be used for this instance.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_number_create(
    vcjson_number** number, RCPR_SYM(allocator)* alloc, double value);

/**
 * \brief Make a deep copy of the given \ref vcjson_number instance.
 *
 * \note On success, this function creates a \ref vcjson_number instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param number        Pointer to the number pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original number to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_number_copy(
    vcjson_number** number, RCPR_SYM(allocator)* alloc,
    const vcjson_number* orig);

/**
 * \brief Get the number value of this \ref vcjson_number instance.
 *
 * \param number        The number instance for this operation.
 *
 * \returns the number value for this instance.
 */
double vcjson_number_value(const vcjson_number* number);

/**
 * \brief Get the resource handle for the given \ref vcjson_number instance.
 *
 * \param number        The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_number_resource_handle(vcjson_number* number);

/**
 * \brief Create a \ref vcjson_string instance using the given allocator and
 * string value.
 *
 * \note On success, this function creates a \ref vcjson_string instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param string        Pointer to the string pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param value         The string value to be used for this instance. This
 *                      value is copied; the original value is assumed owned by
 *                      the caller.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_string_create(
    vcjson_string** string, RCPR_SYM(allocator)* alloc, const char* value);

/**
 * \brief Make a deep copy of the given \ref vcjson_string instance.
 *
 * \note On success, this function creates a \ref vcjson_string instance. This
 * is a resource that is owned by the caller. When no longer needed, this
 * resource must be released by calling \ref resource_release on its resource
 * handle.
 *
 * \param string        Pointer to the string pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original string instance to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_string_copy(
    vcjson_string** string, RCPR_SYM(allocator)* alloc,
    const vcjson_string* orig);

/**
 * \brief Get the string value of this \ref vcjson_string instance.
 *
 * \note The string value returned by this accessor is owned by the
 * \ref vcjson_string instance and its lifetime is the same as the lifetime of
 * that instance.
 *
 * \param string        The string instance for this operation.
 *
 * \returns the string value for this instance.
 */
const char* vcjson_string_value(const vcjson_string* string);

/**
 * \brief Get the resource handle for the given \ref vcjson_string instance.
 *
 * \param string        The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_string_resource_handle(vcjson_string* string);

/**
 * \brief Create a \ref vcjson_value instance from a \ref vcjson_object
 * instance.
 *
 * \note On success, this function creates a \ref vcjson_value instance. The
 * \ref vcjson_object instance that it wraps is owned by this value instance.
 * This value instance is a resource that is owned by the caller. When no longer
 * needed, this resource must be released by calling \ref resource_release on
 * its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param object        The object instance that will be wrapped by this value
 *                      instance on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_object(
    vcjson_value** value, RCPR_SYM(allocator)* alloc, vcjson_object* object);

/**
 * \brief Create a \ref vcjson_value instance from a \ref vcjson_array
 * instance.
 *
 * \note On success, this function creates a \ref vcjson_value instance. The
 * \ref vcjson_array instance that it wraps is owned by this value instance.
 * This value instance is a resource that is owned by the caller. When no longer
 * needed, this resource must be released by calling \ref resource_release on
 * its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param arr           The array instance that will be wrapped by this value
 *                      instance on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_array(
    vcjson_value** value, RCPR_SYM(allocator)* alloc, vcjson_array* arr);

/**
 * \brief Create a \ref vcjson_value instance from a \ref vcjson_number
 * instance.
 *
 * \note On success, this function creates a \ref vcjson_value instance. The
 * \ref vcjson_number instance that it wraps is owned by this value instance.
 * This value instance is a resource that is owned by the caller. When no longer
 * needed, this resource must be released by calling \ref resource_release on
 * its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param number        The number instance that will be wrapped by this value
 *                      instance on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_number(
    vcjson_value** value, RCPR_SYM(allocator)* alloc, vcjson_number* number);

/**
 * \brief Create a \ref vcjson_value instance from a \ref vcjson_string
 * instance.
 *
 * \note On success, this function creates a \ref vcjson_value instance. The
 * \ref vcjson_string instance that it wraps is owned by this value instance.
 * This value instance is a resource that is owned by the caller. When no longer
 * needed, this resource must be released by calling \ref resource_release on
 * its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param string        The string instance that will be wrapped by this value
 *                      instance on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_string(
    vcjson_value** value, RCPR_SYM(allocator)* alloc, vcjson_string* string);

/**
 * \brief Create a \ref vcjson_value instance from the null singleton.
 *
 * \note On success, this function creates a \ref vcjson_value instance which
 * references the null singleton. This value instance is a resource that is
 * owned by the caller. When no longer needed, this resource must be released by
 * calling \ref resource_release on its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_null(
    vcjson_value** value, RCPR_SYM(allocator)* alloc);

/**
 * \brief Create a \ref vcjson_value instance from the true singleton.
 *
 * \note On success, this function creates a \ref vcjson_value instance which
 * references the true singleton. This value instance is a resource that is
 * owned by the caller. When no longer needed, this resource must be released by
 * calling \ref resource_release on its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_true(
    vcjson_value** value, RCPR_SYM(allocator)* alloc);

/**
 * \brief Create a \ref vcjson_value instance from the false singleton.
 *
 * \note On success, this function creates a \ref vcjson_value instance which
 * references the false singleton. This value instance is a resource that is
 * owned by the caller. When no longer needed, this resource must be released by
 * calling \ref resource_release on its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_create_from_false(
    vcjson_value** value, RCPR_SYM(allocator)* alloc);

/**
 * \brief Create a deep copy of the given \ref vcjson_value instance.
 *
 * \note On success, this function creates a \ref vcjson_value instance. This
 * value instance is a resource that is owned by the caller. When no longer
 * needed, this resource must be released by calling \ref resource_release on
 * its resource handle.
 *
 * \param value         Pointer to the value pointer to hold this value.
 * \param alloc         The allocator to use for this operation.
 * \param orig          The original value to copy.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_copy(
    vcjson_value** value, RCPR_SYM(allocator)* alloc, const vcjson_value* orig);

/**
 * \brief Get the value type of this \ref vcjson_value instance.
 *
 * \param value         The value instance for this operation.
 *
 * \returns the value type for this instance.
 */
int vcjson_value_type(const vcjson_value* value);

/**
 * \brief Attempt to get the \ref vcjson_object value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the object pointer to the object value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param object        Pointer to the object pointer to hold the object value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not an object.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_object(vcjson_object** object, vcjson_value* value);

/**
 * \brief Attempt to get the \ref vcjson_array value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the array pointer to the array value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param arr           Pointer to the array pointer to hold the array value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not an array.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_array(vcjson_array** arr, vcjson_value* value);

/**
 * \brief Attempt to get the \ref vcjson_number value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the number pointer to the number value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param number        Pointer to the number pointer to hold the number value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not a number.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_number(vcjson_number** number, vcjson_value* value);

/**
 * \brief Attempt to get the \ref vcjson_string value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the string pointer to the string value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param string        Pointer to the string pointer to hold the string value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not a string.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_string(vcjson_string** string, vcjson_value* value);

/**
 * \brief Attempt to get the \ref vcjson_bool value of this \ref vcjson_value
 * instance.
 *
 * \note On success, this function sets the bool pointer to the bool value
 * of this \ref vcjson_value instance. This pointer is to a value owned by this
 * value instance and the lifetime of this value is the same as the lifetime of
 * this instance.
 *
 * \param boolean       Pointer to the bool pointer to hold the bool value
 *                      on success.
 * \param value         The value instance for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - ERROR_VCJSON_INVALID_GET if the type of this value is not a bool.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_value_get_bool(vcjson_bool** boolean, vcjson_value* value);

/**
 * \brief Get the resource handle for the given \ref vcjson_value instance.
 *
 * \param value         The instance for this accessor.
 *
 * \returns the resource handle for this instance.
 */
RCPR_SYM(resource)* vcjson_value_resource_handle(vcjson_value* value);

/**
 * \brief Attempt to parse a JSON value from a UTF-8 character buffer.
 *
 * \note This parse function must consume all input to be successful.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The input UTF-8 character buffer.
 * \param size          The size of this buffer.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_parse(
    vcjson_value** value, size_t* error_begin, size_t* error_end,
    RCPR_SYM(allocator)* alloc, const char* input, size_t size);

/**
 * \brief Attempt to parse a JSON value from a UTF-8 string.
 *
 * \note This parse function must consume all input to be successful.
 *
 * \param value         Pointer to the value pointer to hold the JSON value on
 *                      success.
 * \param error_begin   Pointer to receive the start of an error location on
 *                      failure.
 * \param error_end     Pointer to receive the end of an error location on
 *                      failure.
 * \param alloc         The allocator to use for this operation.
 * \param input         The UTF-8 input string.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
vcjson_parse_string(
    vcjson_value** value, size_t* error_begin, size_t* error_end,
    RCPR_SYM(allocator)* alloc, const char* input);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
