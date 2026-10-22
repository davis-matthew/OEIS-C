/*
 * A308808  Limiting row sequence of Pascal-like triangle A141021 (with index
 *          of asymmetry s = 4).
 * https://oeis.org/A308808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5) + 2*a(n-4) + a(n-3) + a(n-2) + a(n-1) with the
 *            initial values a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 16
 *            This recurrence computes the limiting row of Pascal‑like
 *            triangle A141021 with asymmetry s = 4 (OEIS A308808)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handles the trivial part of the sequence (a(0)..a(4)). */
    if (n <= 4)
        return pow2(n);   /* pow2(x) = 1 << x for integer types and the
                             corresponding shift for big integers. */

    /* Sliding window of the last five values needed for the recurrence. */
    seq_t last[5];
    for (idx_t i = 0; i <= 4; ++i)
        last[i] = pow2(i);

    /* Compute successive terms up to n. */
    seq_t next;
    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = a(i-5) + 2*a(i-4) + a(i-3) + a(i-2) + a(i-1) */
        next = last[0]          /* a(i-5) */
             + (seq_t)2 * last[1]   /* 2*a(i-4) */
             + last[2]          /* a(i-3) */
             + last[3]          /* a(i-2) */
             + last[4];         /* a(i-1) */

        /* Shift window for the next iteration. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = next;
    }

    return last[4];
}
