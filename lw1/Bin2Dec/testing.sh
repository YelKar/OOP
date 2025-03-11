source ../../testing.sh

assert_args_and_stdin_success() {
  check_test "$(echo "$1" | ./bin2dec)" "$2" 0 $? # STDIN
  check_test "$(./bin2dec "$1")" "$2" 0 $?       # CLI
}

assert_args_and_stdin_failed() {
  check_test "$(echo "$1" | ./bin2dec)" "ERROR" 0 $? # STDIN
  check_test "$(./bin2dec "$1")" "ERROR" 1 $?       # CLI
}

assert_args_and_stdin_success 0 0  # Минимальные значения
assert_args_and_stdin_success 1 1  # Минимальные значения
assert_args_and_stdin_success 10 2  # Небольшое число
assert_args_and_stdin_success 101 5  # Чередование битов
assert_args_and_stdin_success 1111 15  # Единицы (4 бита)
assert_args_and_stdin_success 100000 32  # Единица и нули
assert_args_and_stdin_success 11111111 255  # Единицы (8 бита)
assert_args_and_stdin_success 1010101010 682  # Чередование (10 битов)
assert_args_and_stdin_success 11111111111111111111111111111111 4294967295  # Граничное валидное значение

assert_args_and_stdin_failed 100000000000000000000000000000000  # Граничное невалидное значение
assert_args_and_stdin_failed 10000000000000000000100000000100100  # Значение больше максимального
assert_args_and_stdin_failed 92  # Не двоичное
assert_args_and_stdin_failed Letters  # Не число
assert_args_and_stdin_failed L  # Не число

check_test "$(./bin2dec 1 2)" "ERROR" 1 $?  # Неверное количество аргументов
check_test "$(./bin2dec 1 2 234 2 vd)" "ERROR" 1 $?  # Неверное количество аргументов
check_test "$(echo "" | ./bin2dec)" "ERROR" 0 $?  # Пустая строка
check_test "$(printf "" | ./bin2dec)" "ERROR" 0 $?  # Конец файла

DOC="
Bin2Dec Utility - Version 1.0

Usage:
  bin2dec.exe <binary number>
  replace.exe

Description:
  The utility will convert the number from binary to decimal

Operation Modes:
  1. CLI Args Mode:
     If arguments are provided in the format:
       replace.exe <binary number>
     The utility will perform the following actions:
       - Convert a number from binary to decimal and outputs it to the standard output stream.
       - Output the result to standard output (stdout).
     If the number of arguments is incorrect, the utility:
       - Will output \"ERROR\" to stdout.
       - Will terminate with a return code of 1.

  2. Stdin Mode:
     If no arguments are provided, the utility will read number from standard input (stdin).
     The utility will perform the following actions:
       - Convert a number from binary to decimal and outputs it to the standard output stream.
       - Output the result to standard output (stdout).

Error Handling:
  - In File Mode:
    - If the number of arguments is incorrect, \"ERROR\" is output to stdout, and the program terminates with a code of 1.
    - If the string contains non-binary characters, \"ERROR\" is output to stdout, and the program terminates with a code of 1.
  - In Stdin Mode:
    - If the input is incomplete (e.g., the user presses Ctrl+Z on Windows or Ctrl+D on Linux), \"ERROR\" is output to stdout, and the program terminates with a code of 0.
    - If the string contains non-binary characters, \"ERROR\" is output to stdout, and the program terminates with a code of 0."

check_test "$(./bin2dec -h)" "$DOC" 0 $?
check_test "$(./bin2dec 34 43 -h)" "$DOC" 0 $?
check_test "$(./bin2dec 34 43 -h sdh)" "$DOC" 0 $?


result