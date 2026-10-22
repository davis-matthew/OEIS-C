/*
 * A359800  a(n) is the least m such that the concatenation of n^2 and m is a
 *          square.
 * https://oeis.org/A359800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = least m such that the decimal concatenation of n^2 and m
 *            is a perfect square Computation: for each number of digits d of
 *            m, find the smallest k with k^2 in [n^2·10^d, (n^2+1)·10^d – 1].
 *            The corresponding m = k^2 – n^2·10^d must have exactly d digits.
 *            The minimal such m over all d is returned
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 52, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the square that must appear at the start */
    seq_t sq = (seq_t)n * (seq_t)n;

    seq_t best = 0;          /* current best value for m */
    bool best_set = false;   /* indicates whether best has been assigned */

    seq_t pow10_d = 1;       /* 10^0 initially; will be updated to 10^d in loop */

    for (idx_t d = 1; ; ++d)
    {
        /* Update 10^d */
        pow10_d *= 10ULL;               /* pow10_d = 10^d */
        seq_t pow10_prev = pow10_d / 10ULL; /* 10^(d-1) */

        /* If a smaller m would have a greater number of digits, further d cannot improve */
        if (best_set && pow10_prev > best)
            break;

        /* Interval in which k^2 must lie for the current d */
        seq_t low  = sq * pow10_d;               /* n^2 · 10^d          */
        seq_t high = (sq + 1) * pow10_d - 1;     /* (n^2+1)·10^d – 1    */

        /* Find the range of candidate k values */
        seq_t k_lo = floor_sqrt(low);
        if (k_lo * k_lo < low)
            ++k_lo;

        seq_t k_hi = floor_sqrt(high);
        if (k_lo > k_hi)
            continue;                        /* no k for this d */

        /* Search the few candidates in the tight interval */
        for (seq_t k = k_lo; k <= k_hi; ++k)
        {
            seq_t sqk = k * k;
            seq_t m   = sqk - low;            /* candidate m            */

            /* m must have exactly d decimal digits */
            if (m < pow10_prev || m >= pow10_d)
                continue;

            if (!best_set || m < best)
            {
                best     = m;
                best_set = true;
            }
        }
    }

    return best;
}
