COLOR_ERROR='\033[31m'
COLOR_SUCCESS='\033[32m'
COLOR_RESET='\033[0m'

counter=0
ok=true
unsuccessful_tests=""

check_test() {
  ((counter++))
  local actual_output="$1"
  local expected_output="$2"
  local expected_exit_code="$3"
  local actual_exit_code="$4"

  if [ "$actual_output" != "$expected_output" ]
  then
    echo -e "${COLOR_ERROR}Test $counter failed: Output does not match."
    echo -e "    Expected output: $expected_output"
    echo -e "    Actual output: $actual_output${COLOR_RESET}"
    test_failed
    return 1
  fi

  if [ "$actual_exit_code" -ne "$expected_exit_code" ]
  then
    echo -e "${COLOR_ERROR}Test $counter failed: Exit code does not match."
    echo -e "    Expected exit code: $expected_exit_code"
    echo -e "    Actual exit code: $actual_exit_code${COLOR_RESET}"
    test_failed
    return 1
  fi

  echo -e "${COLOR_SUCCESS}Test $counter passed${COLOR_RESET}"
  return 0
}

test_failed() {
  ok=false
  unsuccessful_tests="$unsuccessful_tests $counter"
}

result() {
  if [ "$ok" == "true" ]
  then
    echo -e "${COLOR_SUCCESS}All tests passed successfully${COLOR_RESET}"
    return 0
  else
    echo -e "${COLOR_ERROR}Some tests failed:${unsuccessful_tests}${COLOR_RESET}"
    return 1
  fi
}
