/*
 * A334671  Number of ways to write n as the sum of a squarefree number
 *          (A005117) and a square (A000290).
 * https://oeis.org/A334671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as the sum of a squarefree
 *            number and a square (allowing the square to be zero)
 * Range:     u8 n <= 63, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   ipow_i32, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_squarefree.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 0;
    seq_t n_sq = (seq_t)n;
    idx_t qmax = (idx_t)floor_sqrt(n_sq);
    for (idx_t q = 0; q <= qmax; ++q) {
        seq_t t    = (seq_t)q * q;
        seq_t rem  = n_sq - t;
        if (rem > 0 && is_squarefree(rem))
            count++;
    }
    return count;
}
