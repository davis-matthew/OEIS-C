/*
 * A374730  a(n) = n * binomial(floor(log_2(n)) + 1, 2).
 * https://oeis.org/A374730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * binomial(floor(log_2(n)) + 1, 2)
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1U)
        return 0;

    /* Compute floor(log2(n)) + 1 by counting shifts */
    unsigned k = 0;
    idx_t t = n;
    while (t > 1U) {
        t >>= 1U;
        k++;
    }
    k++;  /* now k == floor(log2(n)) + 1 */

    seq_t kk = (seq_t)k;
    seq_t choose2 = kk * (kk - 1U) / 2U;
    return (seq_t)n * choose2;
}
