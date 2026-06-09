import ctypes
import os
import sys

# загрузка библиотеки
lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'libintset.so'))
try:
    lib = ctypes.CDLL(lib_path)
except OSError as e:
    print(f"ошибка загрузки {lib_path}: {e}", file=sys.stderr)
    sys.exit(1)

# типы аргументов и возвращаемых значений
lib.int_set_create.argtypes = [ctypes.c_int32, ctypes.c_int32]
lib.int_set_create.restype = ctypes.c_void_p
lib.int_set_destroy.argtypes = [ctypes.c_void_p]
lib.int_set_add.argtypes = [ctypes.c_void_p, ctypes.c_int32]
lib.int_set_add.restype = ctypes.c_bool
lib.int_set_rm.argtypes = [ctypes.c_void_p, ctypes.c_int32]
lib.int_set_rm.restype = ctypes.c_bool
lib.int_set_contains.argtypes = [ctypes.c_void_p, ctypes.c_int32]
lib.int_set_contains.restype = ctypes.c_bool
lib.int_set_cardinality.argtypes = [ctypes.c_void_p]
lib.int_set_cardinality.restype = ctypes.c_size_t
lib.int_set_union.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.int_set_union.restype = ctypes.c_void_p
lib.int_set_intersection.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.int_set_intersection.restype = ctypes.c_void_p
lib.int_set_difference.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.int_set_difference.restype = ctypes.c_void_p
lib.int_set_symmetric_difference.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.int_set_symmetric_difference.restype = ctypes.c_void_p
lib.int_set_is_subset.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.int_set_is_subset.restype = ctypes.c_bool
lib.int_set_is_proper_subset.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.int_set_is_proper_subset.restype = ctypes.c_bool
lib.int_set_equals.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.int_set_equals.restype = ctypes.c_bool
lib.int_set_clear.argtypes = [ctypes.c_void_p]
lib.int_set_is_empty.argtypes = [ctypes.c_void_p]
lib.int_set_is_empty.restype = ctypes.c_bool

def test():
    # создание и базовые операции
    s = lib.int_set_create(0, 20)
    assert s, "create failed"
    lib.int_set_add(s, 5)
    lib.int_set_add(s, 10)
    assert lib.int_set_contains(s, 5)
    assert not lib.int_set_contains(s, 7)
    assert lib.int_set_cardinality(s) == 2
    lib.int_set_rm(s, 5)
    assert not lib.int_set_contains(s, 5)
    assert lib.int_set_cardinality(s) == 1

    # объединение
    a = lib.int_set_create(0, 20)
    b = lib.int_set_create(0, 20)
    lib.int_set_add(a, 1); lib.int_set_add(a, 2)
    lib.int_set_add(b, 2); lib.int_set_add(b, 3)
    u = lib.int_set_union(a, b)
    assert u
    assert lib.int_set_cardinality(u) == 3
    assert lib.int_set_contains(u, 1) and lib.int_set_contains(u, 2) and lib.int_set_contains(u, 3)

    # пересечение
    i = lib.int_set_intersection(a, b)
    assert i and lib.int_set_cardinality(i) == 1 and lib.int_set_contains(i, 2)

    # разность
    d = lib.int_set_difference(a, b)
    assert d and lib.int_set_cardinality(d) == 1 and lib.int_set_contains(d, 1)

    # симметрическая разность
    sd = lib.int_set_symmetric_difference(a, b)
    assert sd and lib.int_set_cardinality(sd) == 2
    assert lib.int_set_contains(sd, 1) and lib.int_set_contains(sd, 3) and not lib.int_set_contains(sd, 2)

    # подмножества
    assert lib.int_set_is_subset(a, u)
    assert not lib.int_set_is_subset(b, a)
    assert lib.int_set_is_proper_subset(a, u)
    assert not lib.int_set_is_proper_subset(a, a)

    # равенство
    a_copy = lib.int_set_create(0, 20)
    lib.int_set_add(a_copy, 1); lib.int_set_add(a_copy, 2)
    assert lib.int_set_equals(a, a_copy)
    assert not lib.int_set_equals(a, b)

    # очистка / пустота
    lib.int_set_clear(a)
    assert lib.int_set_is_empty(a)

    # освобождение
    for x in (s, a, b, u, i, d, sd, a_copy):
        lib.int_set_destroy(x)

    print("все тесты пройдены успешно")

if __name__ == "__main__":
    test()
