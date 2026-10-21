/*
 * A288038  Number of independent vertex sets in the n-Andrásfai graph.
 * https://oeis.org/A288038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (3*n-1)*2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset < 1, return 0 safely */

    seq_t pow2  = seq_pow((seq_t)2, n - 1);   /* 2^(n-1) */
    seq_t coeff = (seq_t)(3 * n - 1);        /* (3*n - 1) */
    return (seq_t)1 + coeff * pow2;         /* 1 + coeff * 2^(n-1) */
}
