// tests.c
#include "feedforward.h"
#include "feedback.h"
#include "printing.h"

#define X 1
#define Y 4
#define Z 11

int main(int argc, char *argv[]) {
  // TEST 1:
  LSTM_type *LSTM = make_LSTM(4, 1);

  // TEST 4/5:
  // Xt_i:
  long double inputs[Z][Y][X] = {
    {{0.0},{0.0},{0.0},{0.0}}, // End of input
    {{0.0},{0.0},{0.0},{1.0}}, // Dummy input
    {{0.0},{0.0},{0.0},{1.0}}, // Dummy input
    {{0.0},{0.0},{0.0},{1.0}}, // Dummy input
    {{0.0},{0.0},{0.0},{1.0}}, // Dummy input
    {{0.0},{0.0},{1.0},{0.0}}, // Output trigger
    {{0.0},{0.0},{1.0},{1.0}}, // First possibility:  NAND(0, 0) = 1
    {{0.0},{1.0},{1.0},{1.0}}, // Second possibility: NAND(0, 1) = 1
    {{1.0},{0.0},{1.0},{1.0}}, // Third possibility:  NAND(1, 0) = 1
    {{1.0},{1.0},{1.0},{1.0}}, // Fourth possibility: NAND(1, 1) = 0
    {{0.0},{0.0},{0.0},{1.0}}  // Dummy input
  };

  // Yt_k:
  long double outputs[Z][Y][X * 2] = {
    {{0.0, 0.0},  // bullshit
     {0.0, 0.0},  // bullshit
     {0.0, 1.0},  // control end (0 ..
     {0.0, 1.0}}, // control end .. 0)
    {{1.0, 0.0},  // answer is 1: (1 ..
     {0.0, 1.0},  // answer is 1: .. 0)
     {0.0, 1.0},  // output answer (0 ..
     {1.0, 0.0}}, // output answer .. 1)
    {{1.0, 0.0},  // answer is 1: (1 ..
     {0.0, 1.0},  // answer is 1: .. 0)
     {0.0, 1.0},  // output answer (0 ..
     {1.0, 0.0}}, // output answer .. 1)
    {{1.0, 0.0},  // answer is 1: (1 ..
     {0.0, 1.0},  // answer is 1: .. 0)
     {0.0, 1.0},  // output answer (0 ..
     {1.0, 0.0}}, // output answer .. 1)
    {{0.0, 1.0},  // answer is 0: (0 ..
     {1.0, 0.0},  // answer is 0: .. 1)
     {0.0, 1.0},  // output answer (0 ..
     {1.0, 0.0}}, // output answer .. 1)
    {{0.0, 0.0},  // bullshit
     {0.0, 0.0},  // bullshit
     {1.0, 0.0},  // control trigger: (1 ..
     {0.0, 1.0}}, // control trigger: .. 0)
    {{0.0, 0.0},  // bullshit
     {0.0, 0.0},  // bullshit
     {1.0, 0.0},  // control input: (1 ..
     {1.0, 0.0}}, // control input: .. 1)
    {{0.0, 0.0},  // bullshit
     {0.0, 0.0},  // bullshit
     {1.0, 0.0},  // control input: (1 ..
     {1.0, 0.0}}, // control input: .. 1)
    {{0.0, 0.0},  // bullshit
     {0.0, 0.0},  // bullshit
     {1.0, 0.0},  // control input: (1 ..
     {1.0, 0.0}}, // control input: .. 1)
    {{0.0, 0.0},  // bullshit
     {0.0, 0.0},  // bullshit
     {1.0, 0.0},  // control input: (1 ..
     {1.0, 0.0}}, // control input: .. 1)
    {{0.0, 0.0},  // bullshit
     {0.0, 0.0},  // bullshit
     {0.0, 1.0},  // output answer (0 ..
     {1.0, 0.0}}  // output answer .. 1)
  };

  // TEST 1:
  NOT_USED(argc);
  NOT_USED(argv);

  /* TEST 2:
  push(LSTM, At_iota, dot_product(first(LSTM, Xt_i), first(LSTM, Wi_iota)));
  */

  /* TEST 3:
  push(LSTM, St_c, 
    sum(2, 
      product(2, 
        matrix_tanh(first(LSTM, At_c)), 
        first(LSTM, Bt_iota)), 
      product(2, 
        second(LSTM, St_c), 
        first(LSTM, Bt_phi))));
  */

  // TEST 5:
  push_all(LSTM, Xt_i, Z, Y, X, (long double *)inputs);
  push_all(LSTM, Xt_i, Z, Y, X * 2, (long double *)outputs);

  // TEST 4:
  feedforward_once(LSTM);

  // TEST 5:
  //feedback_once(LSTM);

  // TEST 1:
  print_LSTM(LSTM);

  // TEST 1:
  LSTM = destroy_LSTM(LSTM);
  return 0;
}
