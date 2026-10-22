/*
 * A389976  Expansion of e.g.f. exp(2*x)*(exp(x) - x).
 * https://oeis.org/A389976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - n*2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    /* a(0) = 1, for n>0 use the explicit formula */
    if (n == 0) return (seq_t)1;

    /* compute 3^n and 2^(n-1) using the generic power routine */
    seq_t pow3 = pow_int((seq_t)3, n);
    seq_t pow2 = pow_int((seq_t)2, n - 1);

    /* multiply n by 2^(n-1) (n is promoted to seq_t) */
    seq_t n_mul_pow2 = (seq_t)n * pow2;

    /* final result */
    return pow3 - n_mul_pow2;
}
