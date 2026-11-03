/*
 * A373028  a(n) is the least positive integer k such that 3*n^2 + 2*n + k^2
 *          is a square.
 * https://oeis.org/A373028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find least k>0 such that 3*n^2 + 2*n + k^2 is a square.
   Equivalent to: let S = 3*n^2 + 2*n.  For each factor pair d*e = S
   with same parity, k = (e - d)/2 is a candidate.  The minimum positive
   k gives a(n).  This algorithm runs in O(sqrt(S)). */
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
    if (n == 0)            /* undefined for index 0 */
        return 0;

    seq_t nn = (seq_t)n;
    seq_t S  = (seq_t)3 * nn * nn + (seq_t)2 * nn;

    seq_t min_k = 0;           /* will hold the minimum found */
    bool found = false;

    seq_t limit = floor_sqrt(S);
    for (seq_t d = 1; d <= limit; ++d)
    {
        if (S % d != 0)
            continue;

        seq_t e = S / d;
        /* parity of both factors must be the same for (e-d) to be even */
        if ((d & 1) != (e & 1))
            continue;

        seq_t k = (e - d) / 2;
        if (k == 0)          /* skip the zero‑k case (d == e) */
            continue;

        if (!found || k < min_k)
        {
            min_k = k;
            found = true;
            if (min_k == 1)   /* cannot be smaller */
                break;
        }
    }

    /* If something goes wrong, return 1 (the smallest possible k). */
    return found ? min_k : (seq_t)1;
}
