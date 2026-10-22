/*
 * A351532  Number of integer pairs (i, j), 0 < i, j < n, such that i/(n - i)
 *          + j/(n - j) = 1.
 * https://oeis.org/A351532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integer pairs (i, j), 0 < i, j < n, * such that
 *            i/(n-i) + j/(n-j) = 1. * Derived: j = n(n-2i) / (2n-3i). For
 *            each i = 1..n-1 * test if the right‑hand side is an integer and
 *            lies * strictly between 0 and n. Count all such pairs
 * Range:     u8 n <= 9952, u16 n <= 9952, u32 n <= 9952, u64 n <= 9952,
 *            u128 n <= 9952, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n <= 1 there are no admissible i, j. */
    if (n <= 1)
        return (seq_t)0;

    /* Count of solutions. */
    seq_t count = 0;

    /* Use 64‑bit signed arithmetic for the intermediate
     * values; products are bounded by ~n^2 <= 1e8 for n up to 10000,
     * so overflow cannot occur. */
    for (idx_t i = 1; i < n; ++i)
    {
        int64_t ii   = (int64_t)i;
        int64_t nn   = (int64_t)n;

        int64_t num  = nn * (nn - 2 * ii);   /* n * (n - 2i) */
        int64_t den  = 2 * nn - 3 * ii;      /* 2n - 3i */

        if (den == 0)
            continue;                       /* avoids division by zero */

        if (num % den != 0)
            continue;                       /* not an integer j */

        int64_t jj = num / den;             /* candidate j */

        if (jj > 0 && jj < nn)
            ++count;                        /* valid ordered pair (i, j) */
    }

    return count;
}
