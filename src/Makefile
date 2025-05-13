CC = gcc
FLAG = -Wall -Wextra -Werror -std=c11
FLAG_LIB = -lcheck -lsubunit -lm -lpthread
FLAG_GCOV = -fprofile-arcs -ftest-coverage
SRC = s21_matrix.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean s21_matrix.a gcov_s21_matrix test gcov_report cppcheck valgrind valgrind-quiet valgrind-errors clang rebuild

all: s21_matrix.a

clean:
	rm -f s21_matrix.a *.o *.h.gch s21_test *.gcda *.gcno *.gcov gcov_test *.info
	rm -rf gcov

s21_matrix.a: $(OBJ)
	$(CC) $(FLAG) -c s21_matrix.h
	ar rcs s21_matrix.a $(OBJ)

gcov_s21_matrix: $(SRC)
	$(CC) $(FLAG_GCOV) $(FLAG) -c $(SRC) s21_matrix.h
	ar rcs s21_matrix.a $(OBJ)

test: clean s21_matrix.a
	$(CC) $(FLAG) s21_test.c s21_matrix.a -o s21_test $(FLAG_LIB)
	./s21_test

gcov_report: clean gcov_s21_matrix
	$(CC) $(FLAG_GCOV) $(FLAG) s21_test.c s21_matrix.c -o gcov_test $(FLAG_LIB)
	./gcov_test
	gcov $(SRC)
	lcov -t "gcov_report" -o s21_matrix_coverage.info -c -d .
	genhtml -o gcov s21_matrix_coverage.info

cppcheck:
	cppcheck --suppress=missingIncludeSystem *.c *.h

valgrind: clean s21_matrix.a
	$(CC) $(FLAG) s21_test.c s21_matrix.a -o s21_test $(FLAG_LIB)
	valgrind --tool=memcheck --leak-check=yes ./s21_test

valgrind-quiet: clean s21_matrix.a
	$(CC) $(FLAG) s21_test.c s21_matrix.a -o s21_test $(FLAG_LIB)
	valgrind --tool=memcheck --leak-check=yes --quiet ./s21_test

valgrind-errors: clean s21_matrix.a
	$(CC) $(FLAG) s21_test.c s21_matrix.a -o s21_test $(FLAG_LIB)
	valgrind --tool=memcheck --leak-check=yes ./s21_test 2>&1 | grep "ERROR SUMMARY"

clang_i:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c *.h

clang:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c *.h

verter:
	bash ../materials/build/run.sh

rebuild: clean all
