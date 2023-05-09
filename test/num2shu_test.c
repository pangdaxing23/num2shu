#include <stdio.h>
#include <string.h>

#include "num2shu_test_cases.h"

static int failed = 0;
static int passed = 0;

static void execute_command(const char* command, char* output, size_t output_size) {
  FILE* pipe = popen(command, "r");
  if (!pipe) {
    printf("Failed to open pipe\n");
    return;
  }

  fgets(output, output_size, pipe);
  pclose(pipe);

  output[strcspn(output, "\n")] = '\0';
}

static void print_test_result(const char* case_output, const char* expected_output) {
  if (strcmp(case_output, expected_output) == 0) {
    printf("Output matches expected: %s\n", case_output);
    passed++;
  } else {
    printf("Output does not match expected:\n");
    printf("Expected: %s\n", expected_output);
    printf("Actual  : %s\n", case_output);
    failed++;
  }

  printf("\n");
}

static void test_num2shu(const char* program_name) {
	int len = test_cases_len;
  char case_output[BUFSIZ];
  char command[BUFSIZ];
	for (int i = 0; i < len; i++) {
     const TestCase* test_case = &test_cases[i];

    const char* input = test_case->input;
    const char* expected_output = test_case->expected_output;

    snprintf(command, sizeof(command), "%s %s", program_name, input);
    printf("Running test case %d:\n", i + 1);
    printf("Command: %s\n", command);
    execute_command(command, case_output, sizeof(case_output));
    print_test_result(case_output, expected_output);
	}
  printf("Passed: %d\nFailed: %d\n", passed, failed);
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    char *program_name = argv[1];
    test_num2shu(program_name);
  } else {
    printf("Usage: %s PROGRAM\n", argv[0]);
    return 1;
  }
  return 0;
}


