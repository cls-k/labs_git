// test.c - тестирование библиотеки int_set

#include "../include/int_set.h"
#include <stdio.h>

static void print_result(const char * name, int passed) {
    printf("%s: %s\n", name, passed ? "OK" : "FAIL");
}

// проверка создания и удаления множества
static void test_create_destroy(void) {
    int_set_t * s = int_set_create(0, 10);
    print_result("create / destroy", s != NULL);
    int_set_destroy(s);
}

// проверка добавления и проверки принадлежности
static void test_add_contains(void) {
    int_set_t * s = int_set_create(0, 10);
    int ok = 1;
    ok = ok && int_set_add(s, 5);
    ok = ok && int_set_contains(s, 5);
    ok = ok && !int_set_contains(s, 7);
    print_result("add / contains", ok);
    int_set_destroy(s);
}

// проверка удаления элемента
static void test_remove(void) {
    int_set_t * s = int_set_create(0, 10);
    int_set_add(s, 5);
    int_set_rm(s, 5);
    int ok = !int_set_contains(s, 5);
    print_result("remove", ok);
    int_set_destroy(s);
}

// проверка подсчёта мощности
static void test_cardinality(void) {
    int_set_t * s = int_set_create(0, 10);
    int_set_add(s, 2);
    int_set_add(s, 4);
    int_set_add(s, 6);
    int ok = (int_set_cardinality(s) == 3);
    print_result("cardinality", ok);
    int_set_destroy(s);
}

// проверка объединения множеств
static void test_union(void) {
    int_set_t * a = int_set_create(0, 10);
    int_set_t * b = int_set_create(0, 10);
    int_set_add(a, 1); int_set_add(a, 2);
    int_set_add(b, 2); int_set_add(b, 3);
    int_set_t * u = int_set_union(a, b);
    int ok = u && int_set_cardinality(u) == 3
            && int_set_contains(u, 1)
            && int_set_contains(u, 2)
            && int_set_contains(u, 3);
    print_result("union", ok);
    int_set_destroy(a);
    int_set_destroy(b);
    int_set_destroy(u);
}

// проверка пересечения множеств
static void test_intersection(void) {
    int_set_t * a = int_set_create(0, 10);
    int_set_t * b = int_set_create(0, 10);
    int_set_add(a, 1); int_set_add(a, 2); int_set_add(a, 3);
    int_set_add(b, 2); int_set_add(b, 3); int_set_add(b, 4);
    int_set_t * i = int_set_intersection(a, b);
    int ok = i && int_set_cardinality(i) == 2
            && int_set_contains(i, 2)
            && int_set_contains(i, 3)
            && !int_set_contains(i, 1);
    print_result("intersection", ok);
    int_set_destroy(a);
    int_set_destroy(b);
    int_set_destroy(i);
}

// проверка разности множеств
static void test_difference(void) {
    int_set_t * a = int_set_create(0, 10);
    int_set_t * b = int_set_create(0, 10);
    int_set_add(a, 1); int_set_add(a, 2); int_set_add(a, 3);
    int_set_add(b, 2); int_set_add(b, 3);
    int_set_t * d = int_set_difference(a, b);
    int ok = d && int_set_cardinality(d) == 1
            && int_set_contains(d, 1)
            && !int_set_contains(d, 2);
    print_result("difference", ok);
    int_set_destroy(a);
    int_set_destroy(b);
    int_set_destroy(d);
}

// проверка симметрической разности
static void test_symmetric_difference(void) {
    int_set_t * a = int_set_create(0, 10);
    int_set_t * b = int_set_create(0, 10);
    int_set_add(a, 1); int_set_add(a, 2);
    int_set_add(b, 2); int_set_add(b, 3);
    int_set_t * sd = int_set_symmetric_difference(a, b);
    int ok = sd && int_set_cardinality(sd) == 2
            && int_set_contains(sd, 1)
            && int_set_contains(sd, 3)
            && !int_set_contains(sd, 2);
    print_result("symmetric difference", ok);
    int_set_destroy(a);
    int_set_destroy(b);
    int_set_destroy(sd);
}

// проверка на подмножество
static void test_subset(void) {
    int_set_t * a = int_set_create(0, 10);
    int_set_t * b = int_set_create(0, 10);
    int_set_t * c = int_set_create(0, 10);
    int_set_add(a, 1); int_set_add(a, 2);
    int_set_add(b, 1); int_set_add(b, 2); int_set_add(b, 3);
    int_set_add(c, 1); int_set_add(c, 2);
    int ok = int_set_is_subset(a, b) && !int_set_is_subset(b, a)
            && int_set_is_subset(a, c) && int_set_is_subset(c, a);
    print_result("subset", ok);
    int_set_destroy(a);
    int_set_destroy(b);
    int_set_destroy(c);
}

// проверка строгого подмножества
static void test_proper_subset(void) {
    int_set_t * a = int_set_create(0, 10);
    int_set_t * b = int_set_create(0, 10);
    int_set_t * c = int_set_create(0, 10);
    int_set_add(a, 1); int_set_add(a, 2);
    int_set_add(b, 1); int_set_add(b, 2); int_set_add(b, 3);
    int_set_add(c, 1); int_set_add(c, 2);
    int ok = int_set_is_proper_subset(a, b)
            && !int_set_is_proper_subset(b, a)
            && !int_set_is_proper_subset(a, c);
    print_result("proper subset", ok);
    int_set_destroy(a);
    int_set_destroy(b);
    int_set_destroy(c);
}

// проверка равенства множеств
static void test_equals(void) {
    int_set_t * a = int_set_create(0, 10);
    int_set_t * b = int_set_create(0, 10);
    int_set_t * c = int_set_create(0, 10);
    int_set_add(a, 1); int_set_add(a, 2);
    int_set_add(b, 1); int_set_add(b, 2);
    int_set_add(c, 1);
    int ok = int_set_equals(a, b) && !int_set_equals(a, c);
    print_result("equals", ok);
    int_set_destroy(a);
    int_set_destroy(b);
    int_set_destroy(c);
}

// проверка пустоты множества и очистки
static void test_empty_clear(void) {
    int_set_t * s = int_set_create(0, 10);
    int_set_add(s, 5);
    int ok = !int_set_is_empty(s);
    int_set_clear(s);
    ok = ok && int_set_is_empty(s);
    print_result("empty / clear", ok);
    int_set_destroy(s);
}

// проверка обработки ошибочных ситуаций
static void test_error_handling(void) {
    int ok = 1;
    ok = ok && (int_set_create(10, 0) == NULL);
    ok = ok && (int_set_add(NULL, 5) == false);
    ok = ok && (int_set_contains(NULL, 5) == false);
    ok = ok && (int_set_cardinality(NULL) == 0);
    int_set_t * s = int_set_create(0, 10);
    ok = ok && (int_set_add(s, 100) == false);
    ok = ok && (int_set_rm(s, 100) == false);
    ok = ok && (int_set_union(s, NULL) == NULL);
    int_set_t * t = int_set_create(5, 15);
    ok = ok && (int_set_union(s, t) == NULL);
    int_set_destroy(s);
    int_set_destroy(t);
    print_result("error handling", ok);
}

int main(void) {
    test_create_destroy();
    test_add_contains();
    test_remove();
    test_cardinality();
    test_union();
    test_intersection();
    test_difference();
    test_symmetric_difference();
    test_subset();
    test_proper_subset();
    test_equals();
    test_empty_clear();
    test_error_handling();
    printf("все тесты завершены\n");
    return 0;
}
