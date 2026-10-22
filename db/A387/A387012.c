/*
 * A387012  Number of ternary strings of length 2*n that have fewer 0's than
 *          the combined number of 1's and 2's.
 * https://oeis.org/A387012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n - Sum_{i=0..n} 2^i * binomial(2*n,i)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 9^n */
    seq_t pow9n = seq_pow((seq_t)9, n);

    /* Sum_{i=0..n} 2^i * binomial(2*n,i) */
    seq_t sum  = (seq_t)0;
    seq_t pow2 = (seq_t)1;          /* current 2^i */
    idx_t two_n = n * (idx_t)2;     /* 2*n, computed in idx_t */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t bin   = binomial(two_n, i);
        sum += pow2 * bin;
        pow2 = pow2 * (seq_t)2;    /* next power of two */
    }

    /* a(n) = 9^n - sum */
    return pow9n - sum;
}
