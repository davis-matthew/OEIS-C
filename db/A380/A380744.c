/*
 * A380744  Number of integers strictly between n^2 and (n+1)^2 with at most
 *          four prime factors (counting multiplicity).
 * https://oeis.org/A380744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k | n² < k < (n+1)² and Ω(k) ≤ 4 } (Ω = total prime
 *            factor count)
 * Range:     u8 n <= 165, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t cnt = 0;
    idx_t low  = n * n + 1;
    idx_t high = (n + 1) * (n + 1) - 1;
    for (idx_t k = low; k <= high; ++k)
    {
        if (bigomega(k) <= 4) ++cnt;
    }
    return cnt;
}
