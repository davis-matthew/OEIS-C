/*
 * A389813  Number of subsets of {1..n} not forming a finite arithmetic
 *          progression.
 * https://oeis.org/A389813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - ( # of arithmetic progression subsets of {1..n} )
 *            Where a progression subset is one whose sorted elements have all
 *            equal successive differences (including subsets of size 0, 1,
 *            2). The count of such subsets is 1 // empty set + n //
 *            singletons + n(n-1)/2 // pairs + Σ_{d=1}^{n-1} Σ_{k=...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t count_AP = 0;

    /* empty set */
    count_AP += (seq_t)1;

    if (n >= 1) {
        /* singletons */
        count_AP += (seq_t)n;

        /* pairs */
        count_AP += (seq_t)n * (seq_t)(n - 1) / 2;
    }

    /* progressions of length ≥ 3 */
    for (idx_t d = 1; d < n; ++d) {
        idx_t maxk = (n - 1) / d;          /* maximal k such that k*d <= n-1 */
        for (idx_t k = 2; k <= maxk; ++k) {
            count_AP += (seq_t)n - (seq_t)k * (seq_t)d;
        }
    }

    /* total subsets = 2^n ; subtract AP subsets */
    return pow2(n) - count_AP;
}
