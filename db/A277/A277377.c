/*
 * A277377  Each odd integer k is followed by k even integers.
 * https://oeis.org/A277377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Let m be the largest integer such that m^2 + m + 1 ≤ n. The odd
 *            terms appear at positions pos = m^2 + m + 1 and have the value
 *            a(pos) = 2m + 1. All other positions contain the even numbers 0,
 *            2, 4, … in order. At an even position n the even‑number index is
 *            e = n – m – 1 (1‑based), so a(n) = 2(e –...
 * Range:     u8 n <= 140, u16 n <= 7162, u32 n <= 7162, u64 n <= 7162,
 *            u128 n <= 7162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)          /* the very first term is 1 */
        return (seq_t)1;

    /* Find the largest m with m^2 + m + 1 ≤ n.
       For n up to 10^4 this linear search is trivial (~100 iterations). */
    idx_t m = 0;
    while ((m + 1) * (m + 2) + 1 <= n)
        ++m;

    idx_t pos = m * m + m + 1;
    if (pos == n)        /* odd position */
        return (seq_t)((idx_t)2 * m + 1);

    /* even position: compute the index among even numbers.  */
    idx_t even_index = n - m - 1;          /* 1‑based */
    seq_t val = (seq_t)2 * ((seq_t)(even_index) - 1);
    return val;
}
