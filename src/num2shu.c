#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#define NUM_UNITS 15
#define NUM_NUMERALS 10
#define SHI_CHARACTER chinese_units[1]
#define ZERO_CHARACTER "零"
#define ZERO_CHARACTER_ALT "〇"
#define ZERO_CHARACTER_RADIO "洞"
#define LIANG_CHARACTER_SC "两"
#define LIANG_CHARACTER_TC "兩"
#define NEGATIVE_CHARACTER_SC "负"
#define NEGATIVE_CHARACTER_TC "負"
#define DIAN_CHARACTER_SC "点"
#define DIAN_CHARACTER_TC "點"

static char *chinese_numerals_sc[NUM_NUMERALS]       = {"", "一", "二", "三", "四", "五", "六", "七", "八", "九"};
static char *chinese_numerals_tc[NUM_NUMERALS]       = {"", "一", "二", "三", "四", "五", "六", "七", "八", "九"};
static char *chinese_numerals_fin_sc[NUM_NUMERALS]   = {"", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};
static char *chinese_numerals_fin_tc[NUM_NUMERALS]   = {"", "壹", "貳", "參", "肆", "伍", "陸", "柒", "捌", "玖"};
static char *chinese_numerals_radio_sc[NUM_NUMERALS] = {"", "幺", "两", "三", "四", "五", "六", "拐", "八", "勾"};
static char *chinese_numerals_radio_tc[NUM_NUMERALS] = {"", "幺", "兩", "三", "四", "五", "六", "拐", "八", "勾"};

static char *chinese_units_sc[NUM_UNITS]             = {"", "十", "百", "千", "万", "亿", "兆", "京", "垓", "秭", "穰", "沟", "涧", "正", "载"};
static char *chinese_units_tc[NUM_UNITS]             = {"", "十", "百", "千", "萬", "億", "兆", "京", "垓", "秭", "穰", "溝", "澗", "正", "載"};
static char *chinese_units_fin_sc[NUM_UNITS]         = {"", "拾", "佰", "仟", "萬", "億", "兆", "京", "垓", "秭", "穰", "沟", "涧", "正", "载"};
static char *chinese_units_fin_tc[NUM_UNITS]         = {"", "拾", "佰", "仟", "萬", "億", "兆", "京", "垓", "秭", "穰", "溝", "澗", "正", "載"};

static char **chinese_numerals = chinese_numerals_sc;
static char **chinese_units = chinese_units_sc;

static char *zero_character = ZERO_CHARACTER;
static char *liang_character = LIANG_CHARACTER_SC;
static char *negative_character = NEGATIVE_CHARACTER_SC;
static char *dian_character = DIAN_CHARACTER_SC;

static int t_flag = 0;
static int f_flag = 0;
static int z_flag = 0;
static int n_flag = 0;
static int s_flag = 0;
static int r_flag = 0;

static int is_negative = 0;
static int is_decimal = 0;

static char *program_name;

void num2shu_dec(const char *const decimal_part) {
  size_t len = strlen(decimal_part);
  printf("%s", dian_character);
  for (size_t i = 0; i < len; i++) {
    if (decimal_part[i] == '0') {
      printf(zero_character);
    }
    else {
      printf("%s", chinese_numerals[decimal_part[i] - '0']);
    }
  }
  printf("\n");
}

int should_use_liang(int i, int j, int block_len, int num_blocks) {
  return (block_len > 2 && j < block_len - 2) || // If numeral for an inner 100 or 1000 unit.
    (num_blocks > 1 && i > 0 && // If 10000 or greater.
     block_len == 1); // Single number in block.
}

void num2shu(const char *const num_str) {
  size_t len = strlen(num_str);
  if (len == 1 && num_str[0] == '0') {
    printf("%s", zero_character);
    printf("\n");
    return;
  }

  if (is_negative) {
    printf("%s", negative_character);
  }

  if (s_flag || r_flag) {
    for (size_t i = 0; i < len; i++) {
      if (num_str[i] == '0') {
        printf("%s", zero_character);
      }
      else {
        printf("%s", chinese_numerals[num_str[i] - '0']);
      }
    }
    return;
  } else {


    int num_blocks = (len % 4 == 0) ? len / 4 : len / 4 + 1;
    char **blocks = malloc(num_blocks * sizeof(char *));
    for (int i = 0; i < num_blocks; i++) {
      int start = len - (i + 1) * 4; // Start scanning from first character of last block
      if (start < 0) {
        start = 0;
      }

      int block_size = (i == num_blocks - 1) ? len % 4 : 4;
      if (block_size == 0) block_size = 4; // For when len is a multiple of 4
      blocks[i] = malloc((block_size + 1) * sizeof(char));
      strncpy(blocks[i], &num_str[start], block_size);
      blocks[i][block_size] = '\0';
    }

    for (int i = num_blocks - 1; i >= 0; i--) { // For blocks in descending magnitude
      int block_len = strlen(blocks[i]);
      for (int j = 0; j < block_len; j++) {
        if (blocks[i][j] != '0') {
          if ((j != 0) ||
              (blocks[i][j] != '1') ||
              (block_len <= 1) ||
              (chinese_units[block_len - 1] != SHI_CHARACTER))
          {
            if (n_flag && blocks[i][j] == '2' && should_use_liang(i, j, block_len, num_blocks)) {
              printf("%s", liang_character);
            }
            else {
              printf("%s", chinese_numerals[blocks[i][j] - '0']);
            }
          }
          printf("%s", chinese_units[block_len - j - 1]);
        }
        else if (j < block_len - 1 && blocks[i][j + 1] != '0') {
          printf("%s", zero_character);
        }
      }
      if (i > 0 && atoi(blocks[i]) != 0) {
        printf("%s", chinese_units[4 + i - 1]);
      }
    }

    if (!is_decimal) {
      printf("\n");
    }

    for (int i = 0; i < num_blocks; i++) {
      free(blocks[i]);
    }
    free(blocks);
  }
}

void check_negative(char *num_str) {
  if (num_str[0] == '-') {
    is_negative = 1;
  }
  if (t_flag) {
    negative_character = NEGATIVE_CHARACTER_TC;
  }
}

char *extract_parts(char *num_str) {
  size_t len = strlen(num_str);
  size_t i = 0;
  size_t j = 0;

  char *decimal_part = malloc((len) * sizeof(char));

  while (i < len) {
    if (isdigit(num_str[i])) {
      num_str[j] = num_str[i];
      j++;
    } else if (num_str[i] == '.') { // Start creating decimal part
      break;
    }
    i++;
  }

  size_t k = 0;
  while (i < len) {
    if (isdigit(num_str[i])) {
      is_decimal = 1;
      decimal_part[k] = num_str[i];
      k++;
    }
    i++;
  }
  num_str[j] = '\0';
  decimal_part[k] = '\0';
  return decimal_part;
}

void trim_leading_zeros(char *num_str) {
  size_t leading_zeros = 0;
  while (num_str[leading_zeros] == '0') {
    leading_zeros++;
  }

  // Leave one zero if the entire string is zeros
  if (leading_zeros > 0 && num_str[leading_zeros] == '\0') {
    leading_zeros--;
  }

  // Shift the rest to the left
  size_t index = 0;
  while (num_str[leading_zeros] != '\0') {
    num_str[index++] = num_str[leading_zeros++];
  }
  num_str[index] = '\0';
}

void check_length(char *num_str) {
  if (strlen(num_str) > 49) {
    printf("Number too large, must be under 1 quindecillion (10^48).");
    exit(1);
  }
}

char *preprocess(char *num_str) {
  check_negative(num_str);
  char *decimal_part = extract_parts(num_str);
  trim_leading_zeros(num_str);
  check_length(num_str);
  return decimal_part;
}

void process_input(char *num_str) {
  char *decimal = preprocess(num_str);
  num2shu(num_str);
  if (strlen(decimal)) {
    num2shu_dec(decimal);
  }
}

void print_help(FILE *out) {
  fprintf(out, "Usage: %s [-htfznsr] <number>\n", program_name);
  fprintf(out, "\nOptions:\n");
  fprintf(out, "  -h, --help        Show this help message and exit.\n");
  fprintf(out, "  -t, --traditional Use traditional numerals and units.\n");
  fprintf(out, "  -f, --financial   Use financial numerals and units. (Overrides -z and -n)\n");
  fprintf(out, "  -z, --zero        Use the alternative \"zero\" character (〇).\n");
  fprintf(out, "  -n, --natural     Use \"两\" for \"two\" in certain contexts.\n");
  fprintf(out, "  -s, --simple      No magnitudes, just simple numeral replacement. (Overrides -n)\n");
  fprintf(out, "  -r, --radio       Like -s, but use unambiguous radio numerals. (Overrides -z)\n");
}

void print_short_help(FILE *out) {
  fprintf(out, "Usage: %s [-htfznsr] <number>\n", program_name);
}

int main(int argc, char *argv[]) {
  program_name = argv[0];

  struct option long_options[] = {
      {"help",        no_argument, 0, 'h'},
      {"traditional", no_argument, 0, 't'},
      {"financial",   no_argument, 0, 'f'},
      {"zero",        no_argument, 0, 'z'},
      {"natural",     no_argument, 0, 'n'},
      {"simple",      no_argument, 0, 's'},
      {"radio",       no_argument, 0, 'r'},
      {0,             0,           0, 0  }
  };

  int opt;
  int option_index = 0;
  while ((opt = getopt_long(argc, argv, "htzfnsr", long_options, &option_index)) != -1) {
    switch (opt) {
    case 't':
      t_flag = 1;
      break;
    case 'f':
      f_flag = 1;
      break;
    case 'z':
      z_flag = 1;
      break;
    case 'n':
      n_flag = 1;
      break;
    case 's':
      s_flag = 1;
      break;
    case 'r':
      r_flag = 1;
      break;
    case 'h':
      print_help(stdout);
      exit(0);
    default:
      print_short_help(stderr);
      exit(1);
    }
  }

  if (f_flag && t_flag) {
    chinese_numerals = chinese_numerals_fin_tc;
    chinese_units = chinese_units_fin_tc;
    n_flag = 0;
    z_flag = 0;
  } else if (f_flag) {
    chinese_numerals = chinese_numerals_fin_sc;
    chinese_units = chinese_units_fin_sc;
    n_flag = 0;
    z_flag = 0;
  } else if (t_flag) {
    chinese_numerals = chinese_numerals_tc;
    chinese_units = chinese_units_tc;
  } else {
    chinese_numerals = chinese_numerals_sc;
    chinese_units = chinese_units_sc;
  }

  if (t_flag) {
    dian_character = DIAN_CHARACTER_TC;
  }

  if (z_flag) {
    zero_character = ZERO_CHARACTER_ALT;
  }

  if (r_flag) {
    if (t_flag) {
      chinese_numerals = chinese_numerals_radio_tc;
    } else {
      chinese_numerals = chinese_numerals_radio_sc;
    }
    zero_character = ZERO_CHARACTER_RADIO;
  }

  if (n_flag && t_flag) {
    liang_character = LIANG_CHARACTER_TC;
  }

  if (optind < argc) {
    char *num_str = argv[optind];
    process_input(num_str);
  } else {
    // No arguments provided, read from stdin
    char buf[BUFSIZ];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
      if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0'; // Replace newline with null terminator
        process_input(buf);
      } else {
        // Line was truncated
        printf("Input line was too long.\n");
        return 1;
      }
    } else {
      printf("Error reading input from stdin.\n");
      return 1;
    }
  }

  return 0;
}
