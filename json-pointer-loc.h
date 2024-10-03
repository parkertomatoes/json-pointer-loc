#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
#define JPLOC_SUCCESS 0
#define JPLOC_NULL_SOURCE 1
#define JPLOC_NULL_PTR 2
#define JPLOC_INVALID_PTR 3
#define JPLOC_NOT_SUPPORTED 4
#define JPLOC_NOT_FOUND 5

typedef struct json_pointer_loc_s {
    size_t position;
    size_t line;
    size_t column;
} json_pointer_loc_t;

extern int json_pointer_loc(
    const char* source, 
    size_t source_length,
    const char* pointer,
    json_pointer_loc_t* location);

extern int json_pointern_loc(
    const char* source, 
    size_t source_length,
    const char* pointer,
    size_t pointer_length,
    json_pointer_loc_t* location);

#ifdef __cplusplus
}
#endif
