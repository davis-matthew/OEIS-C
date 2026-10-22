/*
 * A380024  a(n) = 4^n - 3^n - binomial(n,2)*3^(n-2).
 * https://oeis.org/A380024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n - binomial(n,2)*3^(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute 4^n and 3^n */
    seq_t pow4_n = pow4(n);   /* 4^n   */
    seq_t pow3_n = pow3(n);   /* 3^n   */

    /* 3^(n-2) is defined only for n >= 2 */
    seq_t pow3_n_2 = 0;
    if (n >= 2)
        pow3_n_2 = pow3(n - 2);   /* 3^(n-2) */

    /* binomial(n,2) is 0 for n < 2, safe for all n */
    seq_t binom = binomial(n, (idx_t)2);

    /* a(n) = 4^n - 3^n - binomial(n,2)*3^(n-2) */
    return pow4_n - pow3_n - (binom * pow3_n_2);
}
