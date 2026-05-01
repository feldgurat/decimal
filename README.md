# decimal
Реализация decimal на С, аналог `System.Decimal` из C#.

## Сборка

```bash
cd src
make               # собрать статическую библиотеку s21_decimal.a
make test          # собрать и запустить юнит-тесты (требуется libcheck)
make gcov_report   # отчёт о покрытии кода (требуется lcov)
make memory_test   # проверка утечек памяти (valgrind / leaks)
make format        # проверка стиля кода (clang-format)
make check         # статический анализ (cppcheck)
make clean         # очистка артефактов сборки
```

**Зависимости:** gcc, make, libcheck, lcov, valgrind, cppcheck.

## Реализованные функции

### Арифметика

| Функция | Описание |
|---|---|
| `s21_add` | Сложение |
| `s21_sub` | Вычитание |
| `s21_mul` | Умножение |
| `s21_div` | Деление |

Коды возврата: 0 — OK, 1 — слишком большое число, 2 — слишком малое число, 3 — деление на 0.

### Сравнение

| Функция | Описание |
|---|---|
| `s21_is_less` | Меньше |
| `s21_is_less_or_equal` | Меньше или равно |
| `s21_is_greater` | Больше |
| `s21_is_greater_or_equal` | Больше или равно |
| `s21_is_equal` | Равно |
| `s21_is_not_equal` | Не равно |

Возврат: 0 — FALSE, 1 — TRUE.

### Конвертация

| Функция | Описание |
|---|---|
| `s21_from_int_to_decimal` | int → decimal |
| `s21_from_float_to_decimal` | float → decimal |
| `s21_from_decimal_to_int` | decimal → int |
| `s21_from_decimal_to_float` | decimal → float |

### Округление

| Функция | Описание |
|---|---|
| `s21_floor` | Округление к отрицательной бесконечности |
| `s21_round` | Округление до ближайшего целого |
| `s21_truncate` | Отбрасывание дробной части |
| `s21_negate` | Смена знака |