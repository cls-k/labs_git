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

//функция проверки диапазонов двух множеств и размеров их битовых масок
static bool are_compatible(const int_set_t * set1,
                           const int_set_t * set2) {
    bool result = false;
    if (set1 != NULL && set2 != NULL) {
        if (set1->min_value == set2->min_value &&
            set1->max_value == set2->max_value &&
            set1->byte_count == set2->byte_count) {
            result = true;
        }
    }
    return result;
}

//функция по созданию множества заданного диапазона 
int_set_t * int_set_create(int32_t min_value,
                           int32_t max_value) {

    int_set_t * set = NULL;
    bool        has_error = false;
    size_t      num_bits = 0;
    int64_t     num_bits_ll = 0;

    if (min_value > max_value) {
        has_error = true;
    }

    if (has_error == false) {
        num_bits_ll = (int64_t)max_value - (int64_t)min_value + 1;
        if (num_bits_ll <= 0) {
            has_error = true;
        }
        if (num_bits_ll > (int64_t)SIZE_MAX * 8) {
            has_error = true;
        }
    }

    if (has_error == false) {
        num_bits = (size_t)num_bits_ll;
        set = (int_set_t *)malloc(sizeof(int_set_t));
        if (set == NULL) {
            has_error = true;
        }
    }

    if (has_error == false) {
        set->byte_count = (num_bits + 7) / 8;
        set->min_value = min_value;
        set->max_value = max_value;
        set->bits = (uint8_t *)calloc(set->byte_count, sizeof(uint8_t));
        if (set->bits == NULL) {
            has_error = true;
        }
    }

    if (has_error != false) {
        if (set != NULL) {
            if (set->bits != NULL) {
                free(set->bits);
            }
            free(set); 
        }
        set = NULL;
    }
    return set;
}

//функция уничтоение множества 
void int_set_destroy(int_set_t * set) {
    if (set != NULL) {
        if (set->bits != NULL) {
            free(set->bits);
            set->bits = NULL;
        }
        free(set);
    }
}

//функция добавления элемента в множество
bool int_set_add(int_set_t * set,
                 int32_t     value) {
    bool    has_error = false;
    size_t  byte_idx = 0;
    uint8_t mask = 0;

    if (is_valid_set(set) == false) {
        has_error = true;
    }
    if (has_error == false) {
        if (is_valid_value(set, value) == false) {
            has_error = true;
        }
    }
    if (has_error == false) {
        if (value_to_bit_position(set, value, &byte_idx, &mask) == false) {
            has_error = true;
        }
    }
    if (has_error == false) {
        if (byte_idx >= set->byte_count) {
            has_error = true;
        }
    }
    if (has_error == false) {
        set->bits[byte_idx] |= mask;
    }
    if (has_error != false) {
        has_error = true;
    }
    return has_error;
}

//функция удаления элемента 
bool int_set_rm(int_set_t * set, 
                int32_t value) {
    bool    has_error = false;
    size_t  byte_idx = 0;
    uint8_t mask = 0;

    if (is_valid_set(set) == false) {
        has_error = true;
    }
    if (has_error == false) {
        if (is_valid_value(set, value) == false) {
            has_error = true;
        }
    }
    if (has_error == false) {
        if (value_to_bit_position(set, value, &byte_idx, &mask) == false) {
            has_error = true;
        }
    }
    if (has_error == false) {
        if (byte_idx >= set->byte_count) {
            has_error = true;
        }
    }
    if (has_error == false) {
        set->bits[byte_idx] &= ~mask;
    }
    if (has_error != false) {
        has_error = true;
    }
    return has_error;
}

//функция проверки пренадлежности множества
bool int_set_contains(const   int_set_t * set, 
                      int32_t value) {
    bool    has_error = false;
    bool    result = false;
    size_t  byte_idx = 0;
    uint8_t mask = 0;

    if (is_valid_set(set) == false) {
        has_error = true;
    }
    if (has_error == false) {
        if (is_valid_value(set, value) == false) {
            has_error = true;
        }
    }
    if (has_error == false) {
        if (value_to_bit_position(set, value, &byte_idx, &mask) == false) {
            has_error = true;
        }
    }
    if (has_error == false) {
        if (byte_idx >= set->byte_count) {
            has_error = true;
        }
    }
    if (has_error == false) {
        if ((set->bits[byte_idx] & mask) != 0) {
            result = true;
        }
    }
    if (has_error != false) {
        result = false;
    }
    return result;
}

//функция поиска мощности множества
size_t int_set_cardinality(const int_set_t * set) {
    size_t count = 0;
    size_t iter = 0;
    uint8_t byte = 0;
    if (is_valid_set(set) == false) {
        count = 0;
    }
    if (count != 0) { 
        for (iter = 0; iter < set->byte_count; iter++) {
            byte = set->bits[iter];
            while (byte != 0) {
                count++;
                byte &= (byte - 1);
            }
        }
    }
    return count;
}

//функция объеденения множеств 
int_set_t * int_set_union(const int_set_t * set1,
                          const int_set_t * set2) {
    int_set_t * result = NULL;
    bool        has_error = false;
    size_t      iter = 0;
    if (are_compatible(set1, set2) == false) {
        has_error = true;
    }
    if (has_error == false) {
        result = int_set_create(set1->min_value, set1->max_value);
        if (result == NULL) {
            has_error = true;
        }
    }
    if (has_error == false) {
        for (iter = 0; iter < set1->byte_count; iter++) {
            result->bits[iter] = set1->bits[iter] | set2->bits[iter];
        }
    }
    if (has_error != false) {
        if (result != NULL)
        {
            int_set_destroy(result);
            result = NULL;
        }
    }
    return result;
}

//функция поиска пересечения множеств
int_set_t * int_set_intersection(const int_set_t * set1,
                                 const int_set_t * set2) {
    int_set_t * result = NULL;
    bool        has_error = false;
    size_t      iter = 0;

    if (are_compatible(set1, set2) == false) {
        has_error = true;
    }
    if (has_error == false) {
        result = int_set_create(set1->min_value, set1->max_value);
        if (result == NULL) {
            has_error = true;
        }
    }
    if (has_error == false) {
        for (iter = 0; iter < set1->byte_count; iter++) {
            result->bits[iter] = set1->bits[iter] & set2->bits[iter];
        }
    }
    if (has_error != false) {
        if (result != NULL) {
            int_set_destroy(result);
            result = NULL;
        }
    }
    return result;
}

//функция поиска разности множеств 
int_set_t * int_set_difference(const int_set_t * set1,
                               const int_set_t * set2) {
    int_set_t * result = NULL;
    bool        has_error = false;
    size_t      iter = 0;
    if (are_compatible(set1, set2) == false) {
        has_error = true;
    }
    if (has_error == false) {
        result = int_set_create(set1->min_value, set1->max_value);
        if (result == NULL) {
            has_error = true;
        }
    }
    if (has_error == false) {
        for (iter = 0; iter < set1->byte_count; iter++) {
            result->bits[iter] = set1->bits[iter] & ~set2->bits[iter];
        }
    }
    if (has_error != false) {
        if (result != NULL) {
            int_set_destroy(result);
            result = NULL;
        }
    }
    return result;
}

//функция поиска симметрической разности множеств
int_set_t * int_set_symmetric_difference(const int_set_t * set1,
                                         const int_set_t * set2) {
    int_set_t * result = NULL;
    bool        has_error = false;
    size_t      iter = 0;
    if (are_compatible(set1, set2) == false) {
        has_error = true;
    }
    if (has_error == false) {
        result = int_set_create(set1->min_value, set1->max_value);
        if (result == NULL) {
            has_error = true;
        }
    }
    if (has_error == false) {
        for (iter = 0; iter < set1->byte_count; iter++) {
            result->bits[iter] = set1->bits[iter] ^ set2->bits[iter];
        }
    }
    if (has_error != false) {
        if (result != NULL) {
            int_set_destroy(result);
            result = NULL;
        }
    }
    return result;
}

//функция проверки на подмножество
bool int_set_is_proper_subset(const int_set_t * set1,
                              const int_set_t * set2) {
    bool   result = false;
    bool   has_error = false;
    bool   is_subset = false;
    bool   not_equal = false;
    size_t iter = 0;
    if (are_compatible(set1, set2) == false) {
        has_error = true;
    }
    if (has_error == false) {
        is_subset = int_set_is_subset(set1, set2);
    }
    if (has_error == false && is_subset == true) {
        not_equal = true;
        for (iter = 0; iter < set1->byte_count; iter++) {
            if (set1->bits[iter] == set2->bits[iter]) {
                not_equal = false;
            }
        }
    }
    if (has_error == false && is_subset == true && not_equal == true) {
        result = true;
    }
    return result;
}

//проверка копий множеств
bool int_set_equals(const int_set_t * set1,
                    const int_set_t * set2) {
    bool has_error = false;
    bool result = false;
    if (are_compatible(set1, set2) == false) {
        has_error = true;
    }
    if (has_error == false) {
        if (memcmp(set1->bits, set2->bits, set1->byte_count) == 0) {
            result = true;
        }
    }
    if (has_error != false) {
        result = false;
    }
    return result;
}

//функция печати множества 
void int_set_print(const int_set_t * set) {
    int32_t value = 0;
    int32_t first = 1;
    if (set == NULL) {
        printf("NULL set\n");
        return;
    }
    printf("{ ");
    for (value = set->min_value; value <= set->max_value; value++) {
        if (int_set_contains(set, value) == true) {
            if (first == 0) {
                printf(", ");
            }
            printf("%d", value);
            first = 0;
        }
    }
    printf(" }\n");
}

//функция поиска минимального значения диапаона 
int32_t int_set_min(const int_set_t * set) {
    int result = set->min_value;
    if (set == NULL) {
        result = 0;
    }
    return result;
}

//функция поиска максимального значения диапазона 
int32_t int_set_max(const int_set_t * set) {
    int result = set->max_value;
    if (set == NULL) {
        result = 0;
    }
    return result;
}

//функция поиска байт в битовом пространстве 
size_t int_set_byte_count(const int_set_t * set) {
    int result = 0;
    if (set == NULL) {
        result = 0;
    }
    if (result != 0) { 
        result = set->byte_count;
    }
    return result;
}

//функция проверки множества
void int_set_clear(int_set_t * set) {
    if (is_valid_set(set) == true) {
        memset(set->bits, 0, set->byte_count);
    }
}

//функция проверки множества на пустоту
bool int_set_is_empty(const int_set_t * set) {
    size_t result = 0;
    result = int_set_cardinality(set);
    if (result == 0) {
        result = false;
    }
    return result;
}
