# json-pointer-loc
json-pointer-loc is a small C library for extracting line and column information from JSON elements. Pair it with your favorite JSON parsing library to build better error messages.

```c
// pointer set to something like /section/items/11
const char* pointer = your_api_to_get_error_pointer(err); 
const char* message = your_api_to_get_error_message(err);

json_pointer_loc_t location;
json_pointer_loc(json_text, json_size, pointer, &location);
printf("at line %d, column %d: %s\n", 
    location.line, 
    location.column, 
    message);
```

## Try it out
[WebAssembly Demo](https://parkertomatoes.github.io/json-pointer-loc/)

## Features
 * Finds line and column numbers for elements in a JSON document
 * Compatible with C99
 * Works directly on JSON strings, use with any JSON parsing library
 * Easy integration - single .c and .h file
 * Reasonably performant - scans a large JSON at ~1.25GBps on my Macbook Air M1
 * No dependencies except stdint.h and stddef.h
 * No dynamic allocations, value decoding, or recursion
 * Supports unicode characters in object keys
 * Can compare escaped unicode with directly encoded characters
  * Unicode-aware column counting

## Limitations
 * Negative array indices not yet supported
 * Does NOT validate or parse JSON - must be paired with another library
 
## Who should use this library?
This library was created to build better error messages for applications that use modern JSON parsing libraries. Many applications use JSON for configuration or specification files, and follow this pattern:
 1. Use a library to parse the JSON into a parse tree
 2. Walk the tree to bulid a higher-level data structure
 3. Validate the higher-level data structure

If the JSON file is valid JSON, but is either in the wrong format or has some other constraint failure, the application will report an error. And if you're keen to follow [GNU standards](https://www.gnu.org/prep/standards/html_node/Errors.html), you'll want to report the line and column of any error source. But most JSON parsing libraries shed line and column information after parsing the file. Not providing this information not only makes it harder for users to correct errors, but also makes it harder to integrate your application into text editors or language servers.

This library supplements JSON parser libraries by providing an API to find the line and column of JSON elements. At first glance, it might seem wasteful to re-scan a document source to find error locations rather than using a more complex parsing library. But:
 1. Error reporting is not on the happy path. Taking a few extra cycles in an error scenario is an acceptable trade-off to allow you to use a more optimized parsing library in the common case.
 2. Text scanning is pretty fast, and is often a small part of processing a complex document. 

## Getting Started

Simply add `json-pointer-loc.c` and `json-pointer-loc.h` to your project.

## API Documentation

### json_pointer_loc
Returns location information for a JSON element.
```c
int json_pointer_loc(
    const char* source, 
    size_t source_length,
    const char* pointer,
    json_pointer_loc_t* location);
```
Parameters:
 * *source* - UTF-8 string containing the JSON document
 * *source_length* - Size of the document in bytes
 * *pointer* - UTF-8 string containing the JSON pointer 
 * *location* - (output parameter) Structure to write location information to

Returns a result code, one of the following:
 * *JPLOC_SUCCESS* - Element was located
 * *JPLOC_NOT_FOUND* - Unable to locate JSON pointer
 * *JPLOC_INVALID_PTR* - JSON pointer not formatted correctly
 * *JPLOC_NULL_SOURCE* - JSON source is a null string
 * *JPLOC_NULL_PTR* - JSON pointer is a null string

### json_pointern_loc
Alternative definition of `json_pointer_loc` with an extra parameter to pass pointer string length for non-null-terminated pointer strings, or pointer strings larger than 1024 bytes.

```c
int json_pointern_loc(
    const char* source, 
    size_t source_length,
    const char* pointer,
    size_t pointer_length,
    json_pointer_loc_t* location);
```

### json_pointer_loc_t
Struct containing location information for a JSON element.

Fields:
 * *position* - Byte position of the JSON element
 * *line* - Line number of the JSON element
 * *column* - Column number of the JSON element 

## Running Tests
The µnit sub-repository must be initialized:
```
git submodule update --init --recursive
```

Then, build the test project using CMake:
```
cd test
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

And run the `json-pointer-loc-test` executable.

## Running Benchmarks
The nanobench and test data sub-repositories must be initialized:
```
git submodule update --init --recursive
```

Then, build the benchmark project using CMake
```
cd bench
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

And run the `json-pointer-loc-bench` executable.

## License
This library is MIT licensed.