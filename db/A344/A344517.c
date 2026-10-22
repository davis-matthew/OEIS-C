/*
 * A344517  Minimum diameter of 4-regular circulant graphs of order n.
 * https://oeis.org/A344517
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((sqrt(2n-1)-1)/2)
 * Range:     u8 n <= 32, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the integer square‑root routine provided by the utilities */
#include "../utils/floor_sqrt.h"

/* External linkage, as required */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 4; for smaller n the result
       is irrelevant for the caller.  We return 0 in that case. */
    if (n < 4)
        return 0;

    /* Compute t = 2n - 1 as a seq_t to avoid overflow. */
    seq_t t = (seq_t)2 * n - 1;

    /* Floor of sqrt(t).  The helper guarantees an exact integer. */
    seq_t s = floor_sqrt(t);

    /* Find the smallest odd integer o = 2k+1 with o² ≥ t.
       When s is odd, we check whether s² equals t (exact match);
       otherwise we must take the next odd (s+2).  When s is even,
       the next odd is simply s+1. */
    seq_t k;
    if (s % 2) {            /* s odd */
        seq_t sq = s * s;
        if (sq == t)        /* exact match, use s itself */
            k = (s - 1) / 2;
        else                /* need next odd */
            k = (s + 1) / 2;
    } else {                /* s even, next odd is s+1 */
        k = s / 2;
    }

    return k;
}
