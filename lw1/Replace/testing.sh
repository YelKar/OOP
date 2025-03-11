source ../../testing.sh

assert_args_and_stdin_success() {
  check_test "$(printf "%s\n%s\n%s\n" "$1" "$2" "$3" | ./replace)" "$4" 0 $? # STDIN

  rm testing.in testing.out
  echo "$3" > testing.in
  ./replace testing.in testing.out "$1" "$2"
  return_code="$?"
  check_test "$(cat testing.out)" "$4" 0 $return_code       # CLI
}

# Из примера в "Контесте"
assert_args_and_stdin_success \
    "ma" "mama" \
    "mama delala pelmeni\nya pokushal plotno" "mamamama delala pelmeni\nya pokushal plotno"

# Простая замена
assert_args_and_stdin_success \
    "world" "everyone" \
    "Hello, world!" "Hello, everyone!"

# Замена в начале
assert_args_and_stdin_success \
    "Hello" "Hi" \
    "Hello, world" "Hi, world"

# Замена в конце
assert_args_and_stdin_success \
    "world" "everyone" \
    "Hello, world" "Hello, everyone"

# Многострочная строка
assert_args_and_stdin_success \
    "world" "universe" \
    "Hello, world!\nGoodbye, world!" "Hello, universe!\nGoodbye, universe!"

## Больше вхождений
assert_args_and_stdin_success \
    "world" "Earth" \
    "Hello, world!\nworld is beautiful!\n world\n\nHello, world!" "Hello, Earth!\nEarth is beautiful!\n Earth\n\nHello, Earth!"

# Нет замен
assert_args_and_stdin_success \
    "nothing" "something" \
    "Hello, world!" "Hello, world!"

# Замена на пустую строку
assert_args_and_stdin_success \
    "world" "" \
    "Hello, world!" "Hello, !"

# Замена на такую же строку
assert_args_and_stdin_success \
    "world" "world" \
    "Hello, world!" "Hello, world!"

# Замена на строку содержащую исходную строку
assert_args_and_stdin_success \
    "world" "worldworldworld" \
    "Hello, world!" "Hello, worldworldworld!"
assert_args_and_stdin_success \
    "world" "smthworldsmthworld" \
    "Hello, world!" "Hello, smthworldsmthworld!"

# Пустая изначальная строка
assert_args_and_stdin_success \
    "smth" "smthels" \
    "" ""

# Пустая искомая строка
assert_args_and_stdin_success "" "replace" "input" "input"

# Всё пустое
assert_args_and_stdin_success "" "" "" ""

check_test "$(./replace "first" "second")" "ERROR" 1 $?  # Неверное количество аргументов
check_test "$(./replace "first")" "ERROR" 1 $?  # Неверное количество аргументов
check_test "$(./replace "first" "second" "third" "fourth" "fifth")" "ERROR" 1 $?  # Неверное количество аргументов
check_test "$(./replace "not_existing_file" "second" "third" "fourth")" "ERROR" 1 $?  # файл не найден

# Недостаточно данных
check_test "$(echo "" | ./replace)" "ERROR" 0 $?  # Пустая строка
check_test "$(printf "\n\n" | ./replace)" "ERROR" 0 $?  # Пустая первая строка
check_test "$(printf "First\nSecond" | ./replace)" "ERROR" 0 $?  # Нет изначальной строки
check_test "$(printf "" | ./replace)" "ERROR" 0 $?  # Конец файла

DOC="
Replace Utility - Version 1.0

Usage:
  replace.exe <input file> <output file> <search string> <replace string>
  replace.exe

Description:
  This utility replaces all occurrences of <search string> with <replace string>
  in the provided data and writes the result to the specified output file or outputs it.

Operation Modes:
  1. File Mode:
     If arguments are provided in the format:
       replace.exe <input file> <output file> <search string> <replace string>
     the utility will perform the following actions:
       - Read data from <input file>.
       - Replace all occurrences of <search string> with <replace string>.
       - Write the result to <output file>.
     If the number of arguments is incorrect, the utility:
       - Will output \"ERROR\" to stdout.
       - Will terminate with a return code of 1.

  2. Stdin Mode:
     If no arguments are provided, the utility will read data from standard input (stdin)
     in the following format:
       <search string>
       <replace string>
       <input text>
     The utility will perform the following actions:
       - Replace all occurrences of <search string> with <replace string> in <input text>.
       - Output the result to standard output (stdout).
     If the input is incomplete (e.g., <search string> or <replace string> is missing),
     the utility:
       - Will output \"ERROR\" to stdout.
       - Will terminate with a return code of 0.

Error Handling:
  - In File Mode:
    - If the number of arguments is incorrect, \"ERROR\" is output to stdout, and the program terminates with a code of 1.
    - If the input file cannot be read or the output file cannot be written, \"ERROR\" is output to stdout, and the program terminates with a code of 1.
  - In Stdin Mode:
    - If the input is incomplete (e.g., the user presses Ctrl+Z on Windows or Ctrl+D on Linux), \"ERROR\" is output to stdout, and the program terminates with a code of 0."

check_test "$(./replace -h)" "$DOC" 0 $?
check_test "$(./replace 34 43 -h)" "$DOC" 0 $?
check_test "$(./replace 34 43 -h sdh)" "$DOC" 0 $?


result