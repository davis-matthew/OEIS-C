/*
 * A247644  Triangle formed from the odd-numbered rows of A088855.
 * https://oeis.org/A247644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binom(r-1, floor(k/2))*binom(r-1, ceil(k/2)) where
 *            r=ceil(sqrt(n)) and k = n - (r-1)^2 - 1
 * Range:     u8 n <= 41, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find r such that (r-1)^2 < n <= r^2  (r = ceil(sqrt(n))) */
    idx_t r = 1;
    while (r * r < n) ++r;

    /* number of terms in all previous rows */
    idx_t prev_terms = (r - 1) * (r - 1);

    /* zero‑based position within the current row */
    idx_t j = n - prev_terms - 1;

    /* split j into floor(j/2) and ceil(j/2) */
    idx_t l = j / 2;          /* floor(j/2)   */
    idx_t m = j - l;          /* ceil(j/2)    */

    seq_t a1 = binom(r - 1, l);
    seq_t a2 = binom(r - 1, m);

    return a1 * a2;
}
