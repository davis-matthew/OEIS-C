/*
 * A374382  Number of ways to write n as an ordered sum of a triangular
 *          number, a square and an odd square.
 * https://oeis.org/A374382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (k,m,l) with t = k*(k+1)/2, s = m^2, o
 *            = (2*l+1)^2 such that t + s + o = n. This is exactly the OEIS
 *            definition of A374382
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9776 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

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
    /* cast the index to seq_t; all arithmetic is performed in seq_t to
       avoid retaining large intermediate values.  For small seq_t types
       the arithmetic will wrap as usual, but only those terms that fit
       in seq_t are required to be correct. */
    seq_t N = (seq_t)n;
    seq_t count = 0;

    for (idx_t k = 0; ; ++k) {
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
        if (tri > N) break;

        for (idx_t m = 0; ; ++m) {
            seq_t sq = (seq_t)m * (seq_t)m;
            if (tri + sq > N) break;

            seq_t rem = N - tri - sq;
            seq_t sqrt_rem = floor_sqrt(rem);
            if (sqrt_rem * sqrt_rem == rem && (sqrt_rem & 1) == 1)
                ++count;
        }
    }
    return count;
}
