/*
 * A280073  Numerator of Farey fractions sorted according to increasing k,
 *          with k = numerator + denominator. Fractions with same k are sorted
 *          in order of increasing denominator.
 * https://oeis.org/A280073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280073: Numerator of Farey fractions ordered by increasing k = numerator+denominator
   (fractions with same k are ordered by increasing denominator).
   The algorithm iterates k until the cumulative count reaches n, then scans the
   denominators of the current k to find the n-th fraction.
   Complexity is O(k^2) with k ≈ sqrt(n), which is fine for n ≤ 10000. */
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, safety */
    idx_t target = n;          /* 1‑based target index */
    idx_t cum = 0;             /* cumulative occurrences up to previous k */
    idx_t k = 1;               /* current k value */

    for (;; ++k)
    {
        /* count how many reduced fractions have sum k */
        idx_t count = 0;
        if (k == 1)
        {
            count = 1; /* only 0/1 */
        }
        else
        {
            idx_t start = (k + 1) / 2;        /* ceil(k/2) */
            for (idx_t d = start; d <= k; ++d)
            {
                if (gcd((seq_t)d, (seq_t)(k - d)) == 1)
                    ++count;
            }
        }

        if (cum + count >= target)
        {
            /* the desired fraction is in this block */
            idx_t idx = target - cum;        /* 1‑based index within this k */
            idx_t start = (k + 1) / 2;
            for (idx_t d = start; d <= k; ++d)
            {
                if (gcd((seq_t)d, (seq_t)(k - d)) == 1)
                {
                    if (--idx == 0)
                        return (seq_t)(k - d); /* numerator */
                }
            }
            /* Should not reach here */
            return 0;
        }

        cum += count;
    }
}
