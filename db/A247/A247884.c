/*
 * A247884  Number of positive integers < 10^n divisible by their first digit.
 * https://oeis.org/A247884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum_{L=1..n} sum_{d=1..9} (1 + floor((10^{L-1} - 1)/d))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts positive integers < 10^n that are divisible by their first decimal digit. */
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow10 = 1; /* 10^{L-1} for current L */

    for (idx_t L = 1; L <= n; ++L) {
        seq_t q = pow10 - 1;        /* 10^{L-1} - 1 */
        for (int d = 1; d <= 9; ++d) {
            sum += 1;                      /* the leading number itself */
            sum += q / (seq_t)d;           /* additional multiples in the block */
        }
        pow10 *= 10;            /* prepare 10^{L} for next iteration */
    }
    return sum;
}
