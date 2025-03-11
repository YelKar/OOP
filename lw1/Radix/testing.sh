source ../../testing.sh

assert_success() {
  assert_two_side_success "$1" "$2" "$3" "$4"
  assert_two_side_success "$1" "$2" "-$3" "-$4"
}

assert_two_side_success() {
  assert_one_success "$1" "$2" "$3" "$4"
  assert_one_success "$2" "$1" "$4" "$3"
}

assert_one_success() {
  check_test "$(./radix "$1" "$2" "$3")" "$4" 0 $?
}

assert_failed() {
  check_test "$(./radix "$1" "$2" "$3")" "ERROR" 1 $?       # CLI
}

assert_one_success 2 10 0 0
assert_one_success 2 10 "-0" 0
assert_success 2 10 10 2
assert_success 10 16 254 FE
assert_success 10 36 125 3H
assert_success 16 10 FFFF 65535
assert_success 16 10 7FFFFFFF 2147483647  # Граничные значения
assert_two_side_success 16 10 -80000000 -2147483648  # Граничные значения
assert_failed 16 10 80000000  # Граничные значения
assert_failed 16 10 -80000001  # Граничные значения
assert_failed 37 10 10  # Граничные значения основания
assert_failed 1 10 10  # Граничные значения основания
assert_failed 10 37 10  # Граничные значения основания
assert_failed 10 1 10  # Граничные значения основания
assert_failed 10 ff 10  # Некорректное основание
assert_failed пупупу 10 10  # Некорректное основание
assert_failed 2 10 12  # Некорректное значение
assert_failed 16 10 гг  # Некорректное значение

check_test "$(./radix 1 2)" "ERROR" 1 $?  # Неверное количество аргументов
check_test "$(./radix 1 2 234 2 vd)" "ERROR" 1 $?  # Неверное количество аргументов
check_test "$(echo "" | ./radix)" "ERROR" 1 $?  # Пустая строка
check_test "$(printf "" | ./radix)" "ERROR" 1 $?  # Конец файла

DOC="
Radix Utility - Version 1.0

Usage:
  radix <source notation> <destination notation> <value>

Description:
  The utility converts a number from one numeral system to another.
  Supported numeral systems have bases ranging from 2 to 36. For bases from 11 to 36,
  uppercase Latin letters (A-Z) are used to represent values from 10 to 35.

Parameters:
  <source notation>    - the source numeral system (from 2 to 36).
  <destination notation> - the target numeral system (from 2 to 36).
  <value>              - the number in the source numeral system to be converted.

Examples:
  1. Convert the hexadecimal number 1F to decimal:
     radix 16 10 1F
     Result: 31

  2. Convert the binary number 1010 to octal:
     radix 2 8 1010
     Result: 12

  3. Convert the decimal number -255 to hexadecimal:
     radix 10 16 -255
     Result: -FF

Error handling:
  - If the source or target numeral system is outside the valid range (2-36),
    the program will display an error message and terminate.
  - If the input value contains invalid characters for the specified numeral system,
    the program will display an error message and terminate.
  - If an overflow occurs during conversion, the program will display an error message
    and terminate.

To display this help, use the -h or --help option:
  radix -h
  radix --help"

check_test "$(./radix -h)" "$DOC" 0 $?
check_test "$(./radix 34 43 -h)" "$DOC" 0 $?
check_test "$(./radix 34 43 -h sdh)" "$DOC" 0 $?


result