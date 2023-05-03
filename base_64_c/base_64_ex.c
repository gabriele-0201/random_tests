#include <stdio.h>
#include <stdlib.h>

void print_binary_format(char val) {
  for (int i = 7; i >= 0; i--) {
    char bit = (val & (1 << i)) >> i;
    printf("%d", bit);
  }
}

char *apply_base_64(char *input, int input_len) {
  char *encoded =
      (char *)malloc((input_len / 6) + 1 /*padding*/ + 1 /*terminator*/);

  char mask = (1 << 6) - 1;
  printf("mask: ");
  print_binary_format(mask);
  printf("\n");
  char byte_index = 7;
  int index, index_encoded;

  for (index = 0; index < input_len; index++) {
    print_binary_format(input[index]);
    printf(" ");
  }
  printf("\n");

  for (index = index_encoded = 0; index < input_len; index_encoded++) {
    // 76543210
    // 00000000
    char res;
    if (byte_index < 6) {
      printf("Entered in between \n");
      // 6 bits are divided between two bytes
      char remain_bits = 5 - byte_index;
      printf("remaining bits in next byte %d \n", remain_bits);

      // byte_index - 5 should be always negative
      int index_init_mask = (char)((byte_index - 5) * -1);
      printf("index should begin mask: %d\n'", index_init_mask);
      char shifted_mask = (mask >> index_init_mask);
      printf("proper mask: ");
      print_binary_format(shifted_mask);
      printf("\n");
      char and_result = (input[index] & shifted_mask);
      printf("end with shifted mask result: ");
      print_binary_format(and_result);
      printf("\n");

      res = input[index] & (mask >> index_init_mask);
      res = res << remain_bits;
      printf("init res: ");
      print_binary_format(res);
      printf("\n");
      // restart from the next byte
      byte_index = 7;

      if (index + 1 >= input_len) {
        // IF the input is finished -> zero padding: TODO
      } else {
        index_init_mask = 7 - remain_bits;
        // SOMETHING BROKEN HERE
        char next_res = (input[index + 1] & (((char)((1 << remain_bits) - 1))
                                             << index_init_mask)) >>
                        index_init_mask;
        printf("next byte res: ");
        print_binary_format(next_res);
        printf("\n");

        res |= next_res;
        printf("tot res: ");
        print_binary_format(res);
        printf("\n");
      }

      byte_index = remain_bits + 1;
      index += 1;
    } else {
      // easy case
      printf("Entered in stanadard \n");
      char index_init_mask = byte_index - 5;
      // printf("index should begin mask: %d\n'", index_init_mask);
      // char shifted_mask = (mask << index_init_mask);
      // printf("proper mask: ");
      // print_binary_format(shifted_mask);
      // printf("\n");
      // char and_result = (input[index] & shifted_mask);
      // printf("end with shifted mask result: ");
      // print_binary_format(and_result);
      // printf("\n");
      res = (input[index] & (mask << index_init_mask)) >> index_init_mask;
      byte_index -= 6;
    }
    printf("res: ");
    print_binary_format(res);
    printf("\n");
    encoded[index_encoded] = res;
  }
  encoded[index_encoded] = 0;
}

int main() {
  printf("Insert a random BLA\n");

  int max_in = 1000;
  char *input;

  size_t len = 0;
  ssize_t line_size = 0;
  // HOW THE FUCK THIS COULD WORK?
  line_size = getline(&input, &len, stdin);

  // remove new line character
  input[--line_size] = 0;

  printf("input: %s, size: %d, boh: %d \n", input, line_size, len);

  if (line_size > 1000) {
    printf("line too big for this stupid c program");
    exit(0);
  }

  char *encoded_manually = apply_base_64(input, (int)line_size);

  printf("encoded stuff: %s", encoded_manually);
}
