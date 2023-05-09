#pragma once

#include <stddef.h>

typedef struct {
    char *input;
    char *expected_output;
} TestCase;

extern TestCase test_cases[];
extern size_t test_cases_len;

