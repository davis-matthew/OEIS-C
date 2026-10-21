/*
 * A274099  Number of partitions of n*(n-1)/2 into at most four parts.
 * https://oeis.org/A274099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n*(n-1)/2 into at most four parts
 *            (coefficient of x^(n*(n-1)/2) in 1/((1-x)(1-x^2)(1-x^3)(1-x^4)))
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 131, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   f3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/f3.h"

seq_t a(idx_t n)
{
    /* m = n*(n-1)/2 (up to 64‑bit for n <= 10⁴) */
    seq_t m = (seq_t)n * ((seq_t)n - 1) / 2;

    seq_t sum = 0;
    for (seq_t cur = m; ; cur -= 4) {
        sum += f3(cur);
        if (cur < (seq_t)4) break;
    }
    return sum;
}
