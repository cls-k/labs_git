#include "../include/int_set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

struct int_set
{
    uint8_t * bits;
    size_t     byte_count;
    int32_t    min_value;
    int32_t    max_value;
};

static bool is_valid_set(const int_set_t * set)
{
    bool result = false;

    if (set != NULL)
    {
        if (set->bits != NULL)
        {
            if (set->byte_count > 0)
            {
                result = true;
            }
        }
    }

    return result;
}
