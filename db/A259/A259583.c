/*
 * A259583  Sum of numbers in the n-th antidiagonal of the reciprocity array
 *          of 3.
 * https://oeis.org/A259583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} Sum_{k=0..m-1} floor((n*k + 3)/m)
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   floor_sum_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute floor_sum(n, m, a, b) = Σ_{i=0}^{n-1} ⌊(a*i + b)/m⌋ */
#include <stdint.h>
#include "../utils/floor_sum_ull.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t m = 1; m <= n; ++m) {
        unsigned long long part =
            floor_sum_ull((unsigned long long)m,
                          (unsigned long long)m,
                          (unsigned long long)n,
                          (unsigned long long)3);
        total += (seq_t)part;
    }
    return total;
}
