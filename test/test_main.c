#include <stdlib.h>
#include <string.h>
#include "../json-pointer-loc.h"
#include "munit/munit.h"

MunitResult scalar_value(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "null";
    const char* ptr = "";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 0); // "42"
    return MUNIT_OK;
}

MunitResult object_first_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"a\": 42, \"b\": null}";
    const char* ptr = "/a";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 6); // "42"
    return MUNIT_OK;
}

MunitResult object_second_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"a\": 42, \"b\": null}";
    const char* ptr = "/b";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 15); // "null"
    return MUNIT_OK;
}

MunitResult object_skip_nested(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"a\": [ 42, { \"cherry\": 1.2345 }, [ \"hello\" ] ], \"b\": null}";
    const char* ptr = "/b";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 54); // "null"
    return MUNIT_OK;
}

MunitResult object_nested_object(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"apple\": 42, \"bread\": { \"carrot\": 25, \"donut\": 81 }}";
    const char* ptr = "/bread/donut";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 48); // "81"
    return MUNIT_OK;
}

MunitResult object_nested_array(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"apple\": 42, \"bread\": [ 68, 71, 42 ] }";
    const char* ptr = "/bread/2";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 33); // "42"
    return MUNIT_OK;
}

MunitResult object_empty_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"a\": 42, \"\": null}";
    const char* ptr = "/";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 14); // "null"
    return MUNIT_OK;
}

MunitResult object_unicode_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"a\": 42, \"🧑‍🌾\": null}";
    const char* ptr = "/🧑‍🌾";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 25); // "null"
    return MUNIT_OK;
}

MunitResult object_escaped_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{\"a\": 42, \"\\uD83E\\uDDD1\\u200D\\uD83C\\uDF3E\": null}";
    const char* ptr = "/🧑‍🌾";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 44); // "null"
    return MUNIT_OK;
}

MunitResult array_first_index(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[42, \"a\"]";
    const char* ptr = "/0";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 1); // "null"
    return MUNIT_OK;
}

MunitResult array_second_index(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[42, \"a\"]";
    const char* ptr = "/1";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 5); // "null"
    return MUNIT_OK;
}

MunitResult array_nested_object(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[{ \"hello\": [ null, {\"goodbye\": \"again\" }, 25 ]}, \"a\"]";
    const char* ptr = "/1";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 50);
    return MUNIT_OK;
}

MunitResult array_nested_array(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[{ \"hello\": [ null, {\"goodbye\": \"again\" }, 25 ]}, [ \"a\" ]]";
    const char* ptr = "/1/0";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.position, ==, 52);
    return MUNIT_OK;
}

MunitResult line_counter_simple(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[\n    {\n        \"hello\": [ \n            null,\n            {\n                \"goodbye\": \"again\"\n            },\n            25\n        ]\n    }, \n    \"a\"\n]";
    const char* ptr = "/1";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.line, ==, 11);
    munit_assert_size(loc.column, ==, 5);
    munit_assert_size(loc.position, ==, 147);
    return MUNIT_OK;
}

MunitResult line_counter_unicode(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[\n\"🧑‍🌾\", null]";
    const char* ptr = "/1";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, 0);
    munit_assert_size(loc.line, ==, 2);
    munit_assert_size(loc.column, ==, 9);
    munit_assert_size(loc.position, ==, 17);
    return MUNIT_OK;
}

/* Error cases */

MunitResult error_null_source(const MunitParameter params[], void* user_data_or_fixture) {
    const char* ptr = "/a";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(NULL, 0, ptr, &loc);
    munit_assert_int(result, ==, JPLOC_NULL_SOURCE);
    return MUNIT_OK;
}

MunitResult error_null_pointer(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "null";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, 0, NULL, &loc);
    munit_assert_int(result, ==, JPLOC_NULL_PTR);
    return MUNIT_OK;
}

MunitResult error_invalid_pointer_slash(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{ \"noslash\": 42 }";
    const char* ptr = "noslash";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, JPLOC_INVALID_PTR);
    return MUNIT_OK;
}

MunitResult error_invalid_pointer_escape(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "null";
    const char* ptr = "/hello~2";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, JPLOC_INVALID_PTR);
    return MUNIT_OK;
}

MunitResult error_scalar_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "null";
    const char* ptr = "/a";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, JPLOC_NOT_FOUND);
    return MUNIT_OK;
}

MunitResult error_object_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{ \"banana\": 1, \"cherry\": 2 }";
    const char* ptr = "/apple";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, JPLOC_NOT_FOUND);
    return MUNIT_OK;
}

MunitResult error_array_index(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[ true, true, true ]";
    const char* ptr = "/3";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, JPLOC_NOT_FOUND);
    return MUNIT_OK;
}

MunitResult error_nested_object_key(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "[ false, { \"apple\": 1 } ]";
    const char* ptr = "/1/banana";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, JPLOC_NOT_FOUND);
    return MUNIT_OK;
}

MunitResult error_nested_array_index(const MunitParameter params[], void* user_data_or_fixture) {
    const char* src = "{ \"apple\": [ true, true ] }";
    const char* ptr = "/apple/2";
    json_pointer_loc_t loc;
    int result = json_pointer_loc(src, strlen(src), ptr, &loc);
    munit_assert_int(result, ==, JPLOC_NOT_FOUND);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    // Match cases
    { (char*)"scalar/value", scalar_value, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/first_key", object_first_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/second_key", object_second_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/skip_nested", object_skip_nested, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/nested_object", object_nested_object, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/nested_array", object_nested_array, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/empty_key", object_empty_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/unicode_key", object_unicode_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"object/escaped_key", object_escaped_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"array/first_index", array_first_index, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"array/second_index", array_second_index, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"array/nested_object", array_nested_object, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"array/nested_array", array_nested_array, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"line_counter/simple", line_counter_simple, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"line_counter/unicode", line_counter_unicode, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    // Error cases
    { (char*)"error/null_source", error_null_source, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/null_pointer", error_null_pointer, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/invalid_pointer_slash", error_invalid_pointer_slash, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/invalid_pointer_escape", error_invalid_pointer_escape, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/scalar_key", error_scalar_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/object_key", error_object_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/array_index", error_array_index, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/nested_object_key", error_nested_object_key, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*)"error/nested_array_index", error_nested_array_index, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = { (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&test_suite, NULL, argc, argv);
}