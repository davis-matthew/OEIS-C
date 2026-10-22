/*
 * A380817  a(n) = A379343(A380245(n)).
 * https://oeis.org/A380817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A379343(A380245(n)) Computation uses the triangle T(n,k)
 *            defined in the OEIS entry
 * Range:     u8 n <= 255, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   base_from_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/base_from_row.h"

/* Find the row r such that base(r-1) < n ≤ base(r)  */
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while (base_from_row(hi) < n)
        hi <<= 1;                /* double until we pass n */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (base_from_row(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0; return 0 for safety */
        return (seq_t)0;

    /* Determine row and column */
    idx_t r = find_row(n);
    idx_t prev_base = base_from_row(r - 1);      /* total terms before row r */
    idx_t k = n - prev_base;                    /* 1‑based column in the row */
    idx_t m = 2 * r - 1;                         /* value used in formula */

    /* Compute P(n,k) and then T(n,k) */
    seq_t P;
    if (k < m)
    {
        if (k & 1)  /* odd k */
            P = (seq_t)((m - 1) - k);
        else        /* even k */
            P = (seq_t)((m + 1) - k);
    }
    else
        P = (seq_t)k;

    return (seq_t)prev_base + P;
}
