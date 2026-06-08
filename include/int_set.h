#ifndef INT_SET_H
#define INT_SET_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** @brief битовое множество целых чисел */
typedef struct int_set int_set_t;

/** @brief создать множество для диапазона [min, max] */
int_set_t * int_set_create(int32_t min_value, int32_t max_value);

/** @brief уничтожить множество */
void int_set_destroy(int_set_t * set);

/** @brief добавить элемент */
bool int_set_add(int_set_t * set, int32_t value);

/** @brief удалить элемент */
bool int_set_rm(int_set_t * set, int32_t value);

/** @brief проверить наличие элемента */
bool int_set_contains(const int_set_t * set, int32_t value);

/** @brief количество элементов */
size_t int_set_cardinality(const int_set_t * set);

/** @brief объединение (set1 ∪ set2) */
int_set_t * int_set_union(const int_set_t * set1, const int_set_t * set2);

/** @brief пересечение (set1 ∩ set2) */
int_set_t * int_set_intersection(const int_set_t * set1, const int_set_t * set2);

/** @brief разность (set1 \ set2) */
int_set_t * int_set_difference(const int_set_t * set1, const int_set_t * set2);

/** @brief симметрическая разность (set1 Δ set2) */
int_set_t * int_set_symmetric_difference(const int_set_t * set1, const int_set_t * set2);

/** @brief подмножество (set1 ⊆ set2) */
bool int_set_is_subset(const int_set_t * set1, const int_set_t * set2);

/** @brief строгое подмножество (set1 ⊂ set2) */
bool int_set_is_proper_subset(const int_set_t * set1, const int_set_t * set2);

/** @brief равенство множеств */
bool int_set_equals(const int_set_t * set1, const int_set_t * set2);

/** @brief печать множества (для отладки) */
void int_set_print(const int_set_t * set);

/** @brief минимальное значение диапазона */
int32_t int_set_min(const int_set_t * set);

/** @brief максимальное значение диапазона */
int32_t int_set_max(const int_set_t * set);

/** @brief размер битовой карты в байтах */
size_t int_set_byte_count(const int_set_t * set);

/** @brief очистить множество (удалить все элементы) */
void int_set_clear(int_set_t * set);

/** @brief проверка на пустоту */
bool int_set_is_empty(const int_set_t * set);

#endif
