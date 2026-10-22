/*
 * A310423  Coordination sequence Gal.6.326.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 4 + Σ_{i=0}^{n-2} p[i mod 11] where
 *            the period p of length 11 is: p = {6,4,6,6,6,6,6,4,6,4,4} This
 *            yields the recurrence a(n+1) – a(n) = p[(n-1) mod 11] for n ≥ 1
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* period of the increments, for n≥1:
       a(n+1) - a(n) = period[(n-1) % 11]   */
    static const seq_t period[11] = {6,4,6,6,6,6,6,4,6,4,4};
    const seq_t period_sum = 58; /* sum of the 11 elements */

    idx_t steps = n - 1;      /* number of increments needed from a(1) to a(n) */
    idx_t cycles = steps / 11;
    idx_t rem    = steps % 11;

    seq_t result = 4 + (seq_t)cycles * period_sum;
    for (idx_t i = 0; i < rem; ++i)
        result += period[i];

    return result;
}
