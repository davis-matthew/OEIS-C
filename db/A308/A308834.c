/*
 * A308834  a(n) is the smallest error in trying to solve n^4 = x^4 + y^4.
 *          That is, for each n from 2 on, find positive integers x and y, x
 *          <= y < n such that |n^4 - x^4 - y^4| is minimal and let a(n) = n^4
 *          - x^4 - y^4.
 * https://oeis.org/A308834
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 45 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308834: a(n) is the signed error of the closest representation
   of n⁴ as a sum of two fourth powers with x ≤ y < n.
   Exactly, a(n) = n⁴ - x⁴ - y⁴ where |n⁴ - x⁴ - y⁴| is minimal
   over all 1 ≤ x ≤ y < n.  In a tie (equal |error|) the larger
   error (closer to +0) is chosen. */
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

static inline seq_t fourth_power(idx_t k)
{
    seq_t r = (seq_t)k;
    r = r * r;   /* k² */
    r = r * r;   /* k⁴ */
    return r;
}

static inline seq_t floor_fourth_root(seq_t v)
{
    /* floor( v^(1/4) ) = floor( sqrt( sqrt(v) ) ) */
    return floor_sqrt(floor_sqrt(v));
}

seq_t a(idx_t n)
{
    /* n >= 2 for meaningful calls; behaviour for n < 2 is irrelevant */
    if (n < 2)
        return 0;

    seq_t n4 = fourth_power(n);
    seq_t bestVal = 0;
    seq_t bestDiff = 0;
    bool first = true;

    for (idx_t ix = 1; ix < n; ++ix)
    {
        seq_t x = fourth_power(ix);
        seq_t t = n4 - x;  /* t = n⁴ - x⁴, non‑negative when ix < n */

        seq_t y_est = floor_fourth_root(t);

        for (int d = -1; d <= 1; ++d)
        {
            seq_t y_candidate = y_est + (seq_t)d;

            if (y_candidate < (seq_t)1)
                continue;
            if (y_candidate >= (seq_t)n)
                continue;
            if (y_candidate < (seq_t)ix)   /* enforce x <= y */
                continue;

            idx_t iy = (idx_t)y_candidate;
            seq_t y = fourth_power(iy);

            seq_t cand = n4 - x - y;                       /* n⁴ - x⁴ - y⁴ */
            seq_t absCand = cand >= 0 ? cand : -cand;       /* |cand| */

            if (first ||
                absCand < bestDiff ||
                (absCand == bestDiff && cand > bestVal))
            {
                bestDiff = absCand;
                bestVal  = cand;
                first    = false;
            }

            if (!first && bestDiff == 0)   /* exact match found */
                goto done;
        }
    }
done:
    return bestVal;
}
