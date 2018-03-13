// _feedback.c
#include "_feedback.h"

void feedback(LSTM_type *LSTM) {
  matrix_type *DZt_plus_1 = first(LSTM, DZt), 
              *DIt_plus_1 = first(LSTM, DIt), 
              *DFt_plus_1 = first(LSTM, DFt), 
              *DOt_plus_1 = first(LSTM, DOt), 
              *DCt_plus_1 = first(LSTM, DCt), 
              *Ft_plus_1  = first(LSTM, Ft);

  matrix_for_each(zero, DZt_plus_1);
  matrix_for_each(zero, DIt_plus_1);
  matrix_for_each(one,  DFt_plus_1);
  matrix_for_each(zero, DOt_plus_1);
  matrix_for_each(zero, DCt_plus_1);
  matrix_for_each(zero, Ft_plus_1);

  copy_tensor(LSTM, Input,  Xt);
  copy_tensor(LSTM, Output, Answer);

  //while (LSTM->tensor[Answer].time) {
    // FEEDBACK PART:
    // Block output errors:
    push(LSTM, DYt, 
      sum(5, 
        subtract(2, 
          pop(LSTM, Answer), 
          first(LSTM, Yt)), 
        dot_product(
          transpose(first(LSTM, Rz)),
          matrix_copy(DZt_plus_1)), 
        dot_product(
          transpose(first(LSTM, Ri)),
          matrix_copy(DIt_plus_1)), 
        dot_product(
          transpose(first(LSTM, Rf)),
          matrix_copy(DFt_plus_1)), 
        dot_product(
          transpose(first(LSTM, Ro)),
          matrix_copy(DOt_plus_1))));

    // Output gate errors:
    push(LSTM, DOt, 
      product(3, 
        first(LSTM, DYt), 
        matrix_tanh(first(LSTM, Ct)), 
        sigmoid_derivative(first(LSTM, _Ot))));

    // Cell memory errors:
    push(LSTM, DCt, 
      sum(5, 
        product(3, 
          first(LSTM, DYt), 
          first(LSTM, Ot), 
          tanh_derivative(first(LSTM, Ct))), 
        product(2, 
          first(LSTM, Po), 
          first(LSTM, DOt)), 
        product(2, 
          first(LSTM, Pi), 
          matrix_copy(DIt_plus_1)), 
        product(2, 
          first(LSTM, Pf), 
          matrix_copy(DFt_plus_1)), 
        product(2, 
          DCt_plus_1, 
          Ft_plus_1)));

    // Forget gate errors:
    push(LSTM, DFt, 
      product(3, 
        first(LSTM, DCt), 
        second(LSTM, Ct), 
        sigmoid_derivative(first(LSTM, _Ft))));

    // Input gate errors:
    push(LSTM, DIt, 
      product(3, 
        first(LSTM, DCt), 
        first(LSTM, Zt), 
        sigmoid_derivative(first(LSTM, _It))));

    // Block input errors:
    push(LSTM, DZt, 
      product(3, 
        first(LSTM, DCt), 
        first(LSTM, It), 
        tanh_derivative(first(LSTM, _Zt))));

    // UPDATE PART:
    // Block input input weight updates:
    push(LSTM, DWz, 
      sum(2, 
        pop(LSTM, DWz), 
        dot_product(
          transpose(first(LSTM, DZt)), 
          first(LSTM, Xt))));

    // Input gate input weight updates:
    push(LSTM, DWi, 
      sum(2, 
        pop(LSTM, DWi), 
        dot_product(
          transpose(first(LSTM, DIt)), 
          first(LSTM, Xt))));

    // Forget gate input weight updates:
    push(LSTM, DWf, 
      sum(2, 
        pop(LSTM, DWf), 
        dot_product(
          transpose(first(LSTM, DFt)), 
          first(LSTM, Xt))));

    // Output gate input weight updates:
    push(LSTM, DWo, 
      sum(2, 
        pop(LSTM, DWo), 
        dot_product(
          transpose(first(LSTM, DOt)), 
          first(LSTM, Xt))));

    // Block input recurrent weight updates:
    push(LSTM, DRz, 
      sum(2, 
        pop(LSTM, DRz), 
        dot_product(
          transpose(DZt_plus_1), 
          first(LSTM, Yt))));

    // Input gate recurrent weight updates:
    push(LSTM, DRi, 
      sum(2, 
        pop(LSTM, DRi), 
        dot_product(
          transpose(matrix_copy(DIt_plus_1)), 
          first(LSTM, Yt))));

    // Forget gate recurrent weight updates:
    push(LSTM, DRf, 
      sum(2, 
        pop(LSTM, DRf), 
        dot_product(
          transpose(matrix_copy(DFt_plus_1)), 
          first(LSTM, Yt))));

    // Block output recurrent weight updates:
    push(LSTM, DRo, 
      sum(2, 
        pop(LSTM, DRo), 
        dot_product(
          transpose(DOt_plus_1), 
          first(LSTM, Yt))));

    // Input gate peephole weight updates:
    push(LSTM, DPi, 
      sum(2, 
        pop(LSTM, DPi), 
        product(2, 
          first(LSTM, Ct), 
          DIt_plus_1)));

    // Forget gate peephole weight updates:
    push(LSTM, DPf, 
      sum(2, 
        pop(LSTM, DPf), 
        product(2, 
          first(LSTM, Ct), 
          DFt_plus_1)));

    // Output gate peephole weight updates:
    push(LSTM, DPo, 
      sum(2, 
        pop(LSTM, DPo), 
        product(2, 
          first(LSTM, Ct), 
          first(LSTM, DOt))));

    // Block input bias weight updates:
    push(LSTM, DBz, 
      sum(2, 
        pop(LSTM, DBz), 
        first(LSTM, DZt)));

    // Input gate bias weight updates:
    push(LSTM, DBi, 
      sum(2, 
        pop(LSTM, DBi), 
        first(LSTM, DIt)));

    // Forget gate bias weight updates:
    push(LSTM, DBf, 
      sum(2, 
        pop(LSTM, DBf), 
        first(LSTM, DFt)));

    // Output gate bias weight updates:
    push(LSTM, DBo, 
      sum(2, 
        pop(LSTM, DBo), 
        first(LSTM, DOt)));

    // Prepare next iteration:
    
  //}
}

/*
void feedback_once(LSTM_type *LSTM, unsigned int epoch) {
  matrix_type *net_error = 
    subtract(2, matrix_copy(LSTM->tensor[Yt_k].matrix[epoch - 1]), 
                second(LSTM, Bt_c));

  // Output errors:
  push(LSTM, Dt_k, 
    sum(5, matrix_copy(net_error), 
           dot_product(first(LSTM, Dt_omega), 
                       transpose(first(LSTM, Wh_omega))), 
           dot_product(first(LSTM, Dt_c), 
                       transpose(first(LSTM, Wh_c))), 
           dot_product(first(LSTM, Dt_phi), 
                       transpose(first(LSTM, Wh_phi))), 
           dot_product(first(LSTM, Dt_iota), 
                       transpose(first(LSTM, Wh_iota)))));

  net_error = destroy_matrix(net_error);

  // Output gate errors:
  push(LSTM, Dt_omega, 
    product(2, sigmoid_derivative(second(LSTM, At_omega)), 
               dot_product(first(LSTM, Dt_k), 
                           transpose(matrix_tanh(second(LSTM, St_c))))));

  // Cell state errors:
  push(LSTM, Dt_s, 
    sum(5, product(3, second(LSTM, Bt_omega), 
                      sigmoid_derivative(second(LSTM, St_c)), 
                      first(LSTM, Dt_k)), 
           product(2, first(LSTM, Bt_phi), 
                      second(LSTM, Dt_s)), 
           product(2, transpose(first(LSTM, Wc_iota)), 
                      second(LSTM, Dt_iota)), 
           product(2, transpose(first(LSTM, Wc_phi)), 
                      second(LSTM, Dt_phi)), 
           product(2, transpose(first(LSTM, Wc_omega)), 
                      first(LSTM, Dt_omega))));

  // Cell output errors:
  push(LSTM, Dt_c, 
    product(3, second(LSTM, Bt_iota), 
               tanh_derivative(second(LSTM, At_c)), 
               first(LSTM, Dt_s)));

  // Forget gate errors:
  push(LSTM, Dt_phi, 
    product(2, sigmoid_derivative(second(LSTM, At_phi)), 
               dot_product(first(LSTM, Dt_s), 
                           transpose(third(LSTM, St_c)))));

  // Input gate errors:
  push(LSTM, Dt_iota, 
    product(2, sigmoid_derivative(second(LSTM, At_iota)), 
               dot_product(first(LSTM, Dt_s), 
                           transpose(matrix_tanh(second(LSTM, At_c))))));
}
*/
