CC = gcc #компилятор 
CFLAGS = -Wall -Wextra -Wpedantic -fPIC #флаги: предупреждения, строгий стандарт, позиционно независимый код 
INCLUDE = -Iinclude #пусть к заголовочному файлу (папка include)
LIB_SRC = src/int_set.c #исходный файл библиотеки, где реализованы сами функции 
LIB_OBJ = src/int_set.o #скомпилированный файл
LIB_STATIC = libint_set.a #имя статической библиотеки 
LIB_SHARED = libint_set.so #имя динамической билиотеки

all: $(LIB_STATIC) $(LIB_SHARED) #собрание обеих библиотек 

$(LIB_SHARED): $(LIB_OBJ) #цель по умолчанию: собрать обе библиотеки
	$(CC) -shared -o $@ $^

$(LIB_STATIC): $(LIB_OBJ) #статическая библиотека зависит от объектного файла
	ar rcs $@ $^

$(LIB_OBJ): $(LIB_SRC) include/int_set.h #объектный файл зависит от .c и .h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ #скомпилировать исходник в объектный (-c), $< — первая зависимость

test: $(LIB_STATIC) #цель test зависит от статической библиотеки
	$(CC) $(CFLAGS) $(INCLUDE) tests/test.c -L. -lint_set -o tests/test
	./tests/test #запустить скомпилированный тест

py-test: $(LIB_SHARED)
	PYTHONPATH=. python3 tests/test.py 
# установить путь к .so и запустить питон тест

clean:
	rm -f $(LIB_OBJ) $(LIB_STATIC) $(LIB_SHARED) tests/test # удалить все сгенерированные файл

.PHONY: all clean test py-test # объявить цели, которые не создают файлы (всегда выполняются)
