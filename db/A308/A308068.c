/*
 * A308068  Number of integer-sided triangles with perimeter n whose longest
 *          side length is even.
 * https://oeis.org/A308068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – number of integer‑sided triangles with perimeter n whose longest side is even.
   For each possible even longest side c (ceil(n/3) ≤ c ≤ floor((n-1)/2)),
   let S = n - c be the sum of the other two sides.
   The admissible values of the smallest side a satisfy:
            max(1, S - c) ≤ a ≤ floor(S/2).
   The number of such a is max(0, floor(S/2) - (S - c) + 1).
   Summing these counts over all even c yields a(n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases automatically by the loop below */
    idx_t min_c = (n + 2) / 3;        /* ceil(n/3)   */
    idx_t max_c = (n - 1) / 2;        /* floor((n-1)/2) */
    seq_t result = 0;

    for (idx_t c = min_c; c <= max_c; ++c)
    {
        if (c & 1)          /* skip odd longest side */
            continue;

        idx_t S = n - c;          /* sum of the two smaller sides */
        idx_t lower = S - c;      /* a ≥ S - c  (guaranteed ≥1 here) */
        idx_t upper = S / 2;      /* a ≤ floor(S/2) */

        if (upper >= lower)
            result += (seq_t)(upper - lower + 1);
    }

    return result;
}
