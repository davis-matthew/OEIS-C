/*
 * A394314  a(n) = 2^(2*n-1) - 3^n + 2^n - 1, n > 0; a(0)=0.
 * https://oeis.org/A394314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n-1) - 3^n + 2^n - 1 (n > 0); a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Exponent for the 4^n term: 2*n - 1 */
    idx_t exp_pow4 = 2 * n - 1;

    /* Compute the individual terms */
    seq_t term_pow4 = pow2(exp_pow4); /* 2^(2*n-1) */
    seq_t term_pow2 = pow2(n);        /* 2^n */
    seq_t term_pow3 = pow3(n);        /* 3^n */

    /* a(n) = term_pow4 + term_pow2 - (term_pow3 + 1) */
    seq_t result = term_pow4;
    result += term_pow2;
    result -= (term_pow3 + (seq_t)1);

    return result;
}
