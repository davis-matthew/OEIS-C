/*
 * A363028  Expansion of Sum_{k>0} k * x^(2*k-1) / (1 - x^(5*k-3)).
 * https://oeis.org/A363028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * \sum_{d | (5*n-1), d ≡ 2 (mod 5)} (d+3)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Sequence entry a(n) for A363028, offset 1. */
seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1.  Return 0 for safety. */
    if (n == 0)
        return 0;

    /* m = 5*n - 1.  For the target range this fits in uint64_t. */
    uint64_t m = (uint64_t)5 * n - 1;

    seq_t sum = 0;
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t d1 = d;
            if (d1 % 5 == 2)
                sum += (seq_t)d1 + 3;

            uint64_t d2 = m / d;
            if (d2 != d1 && d2 % 5 == 2)
                sum += (seq_t)d2 + 3;
        }
    }

    /* sum is always a multiple of 5. */
    return sum / 5;
}
