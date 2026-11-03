/*
 * A344619  The a(n)-th composition in standard order (A066099) has
 *          alternating sum 0.
 * https://oeis.org/A344619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonnegative integer k for which the composition
 *            obtained from k in the standard order of A066099 has alternating
 *            sum 0. Thus a(1)=0, a(2)=3, a(3)=10,
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Determines whether the composition corresponding to k (as
   described in the OEIS entry) has alternating sum zero. */
static bool alt_sum_zero(idx_t k)
{
    /* Array to hold the part sizes of the composition. */
    idx_t parts[64];
    idx_t m = 0;          /* number of parts */

    idx_t x   = k;        /* working copy of k */
    idx_t pos = 1;        /* position (1‑based) of bit being examined */
    idx_t prev = 0;       /* previous set‑bit position */

    while (x) {
        if (x & 1U) {
            parts[m++] = pos - prev;
            prev = pos;
        }
        ++pos;
        x >>= 1;
    }

    /* Compute alternating sum of the reversed parts. */
    long long sum = 0;
    for (idx_t i = 0; i < m; ++i) {
        idx_t p = parts[m - 1 - i];
        if (i & 1U)
            sum -= (long long)p;
        else
            sum += (long long)p;
    }
    return sum == 0;
}

seq_t a(idx_t n)
{
    /* Find the n‑th index k (starting from 0) with alternating sum 0. */
    idx_t count = 0;
    for (idx_t k = 0;; ++k) {
        if (alt_sum_zero(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
