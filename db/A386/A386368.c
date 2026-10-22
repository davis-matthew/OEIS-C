/*
 * A386368  a(n) = Sum_{k=0..n-1} binomial(6*k,k) * binomial(6*n-6*k-2,n-k-1).
 * https://oeis.org/A386368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386368
   Formula: a(n) = Sum_{k=0..n-1} binomial(6*n-1, k) * 5^(n-1-k)
   (equivalent to the original definition).
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* 5^(n-1) */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow5 *= 5;

    seq_t binom = 1;          /* binomial(6*n-1, 0) */
    seq_t result = 0;

    for (idx_t k = 0; k < n; ++k) {
        /* add current term */
        result += binom * pow5;

        /* next 5^(n-1-(k+1)) */
        pow5 /= 5;

        /* next binomial coefficient */
        if (k + 1 < n) {
            idx_t num = 6 * n - 1 - k;
            idx_t den = k + 1;
            /* use 128‑bit intermediate to avoid overflow of
               the intermediate product when seq_t is 64‑bit */
            unsigned __int128 tmp = (unsigned __int128)binom * num;
            binom = (seq_t)(tmp / den);
        }
    }

    return result;
}
