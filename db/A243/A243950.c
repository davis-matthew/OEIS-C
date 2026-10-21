/*
 * A243950  Sum of the squares of q-binomial coefficients for q=2 in row n of
 *          triangle A022166, for n >= 0.
 * https://oeis.org/A243950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n} ([n k]_2)^2 where [n k]_2 =
 *            product_{i=1}^{k} (2^{n+1-i} - 1) / (2^{i} - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Use multiplicative recurrence for q-binomial coefficients.
       coeff_k = [n k]_2.  coeff_{k+1} = coeff_k * (2^{n-k} - 1)/(2^{k+1} - 1). */
    seq_t sum    = 0;           /* accumulates sum of squares */
    seq_t coeff  = 1;           /* coeff_0 = 1 */

    seq_t pow2_nk = pow2(n);    /* 2^{n-k} for current k, starts with 2^n. */
    seq_t pow2_kp1 = 2;         /* 2^{k+1} for current k, starts with 2^1. */

    for (idx_t k = 0; k <= n; ++k) {
        /* add coeff^2 to sum */
        sum += coeff * coeff;

        if (k == n) break;    /* done with last coefficient */

        /* Prepare next coefficient */
        seq_t numerator   = pow2_nk - 1;   /* 2^{n-k} - 1 */
        seq_t denominator = pow2_kp1 - 1;  /* 2^{k+1} - 1 */
        coeff = (coeff * numerator) / denominator;

        /* Update exponent values for next iteration */
        pow2_nk >>= 1;          /* 2^{n-(k+1)} = 2^{n-k} / 2 */
        pow2_kp1 <<= 1;         /* 2^{(k+1)+1} = 2^{k+1} * 2 */
    }

    return sum;
}
