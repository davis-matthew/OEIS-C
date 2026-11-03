/*
 * A267140  Least m>0 for which m + n^2 is a square and m + triangular(n) is a
 *          triangular number (A000217).
 * https://oeis.org/A267140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 4112, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9952 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using a simple O(n) search: for each k > n compute
   m = k^2 - n^2 and test that m + n^2 is a square
   and m + triangular(n) is a triangular number.  The first
   k that satisfies both gives the minimal m.  For n > 1
   the known upper bound k ≤ 5n suffices; for n = 1 the bound
   is 6.  n = 0 returns 1 directly. */
#include <stdbool.h>
#include <stdint.h>
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
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t N  = (seq_t)n;                 /* cast to seq_t */
    seq_t Tn = N * (N + 1) / 2;          /* triangular number of n */

    /* Upper bound for k:
       For n > 1 the bound k ≤ 5n is guaranteed,
       but for n = 1 we need k = 6.  Hence use 6n for safety. */
    idx_t maxk = 6 * n;
    for (idx_t k = n + 1; k <= maxk; ++k) {
        seq_t K = (seq_t)k;
        seq_t m = K * K - N * N;          /* m > 0 */

        /* Check that m + Tn is a triangular number:
           condition: 1 + 8*(m + Tn) is an odd perfect square. */
        seq_t D = 1 + 8 * (m + Tn);
        seq_t s = floor_sqrt(D);
        if (s * s == D && (s & 1))
            return m;
    }

    /* In the very unlikely event the bound is not sufficient,
       continue searching beyond it. */
    for (idx_t k = maxk + 1; ; ++k) {
        seq_t K = (seq_t)k;
        seq_t m = K * K - N * N;
        seq_t D = 1 + 8 * (m + Tn);
        seq_t s = floor_sqrt(D);
        if (s * s == D && (s & 1))
            return m;
    }
}
