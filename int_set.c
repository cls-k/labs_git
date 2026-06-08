#include "../include/int_set.h" 
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <stdint.h>

struct int_set {
    uint8_t * bits;
    size_t     byte_count;
    int32_t    min_value;
    int32_t    max_value;
};

//функция проверки существования множества 
//и проверка на то, что он ненулевой
static bool is_valid_set(const int_set_t * set) {
    bool result = false;
    if (set != NULL) {
        if (set->bits != NULL) {
            if (set->byte_count > 0) {
                result = true;
            }
        }
    }
    return result;
}

//функция проверки вхождения числа в множество 
static bool is_valid_value(const int_set_t * set,
                           int32_t value) {
    bool result = false;
    if (set != NULL) {
        if (value >= set->min_value && value <= set->max_value) {
            result = true;
        }
    }
    return result;
}

//функция вычисления индекса байта и битовой маски в битовом массиве
static bool value_to_bit_position(const int_set_t * set,
                                  int32_t         value,
                                  size_t *        byte_idx,
                                  uint8_t *       mask) {
    bool   result = false;
    int32_t offset = 0;
    if (set != NULL && byte_idx != NULL && mask != NULL) {
        offset = value - set->min_value;
        *byte_idx = (size_t)(offset / 8);
        *mask = (uint8_t)(1 << (offset % 8));
        result = true;
    }
    return result;
}

