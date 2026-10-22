/*
 * A310341  Coordination sequence Gal.6.527.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For n≥1, a(n)=4 + Σ_{i=0}^{n-2} p_i * where the periodic
 *            difference pattern p repeats every 7 terms: * p =
 *            [6,2,6,2,6,4,4]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0U) return 1;

    /* m = n-1 is the number of differences from a(1) to a(n) */
    idx_t m = n - 1U;
    idx_t cycles_i = m / 7U;
    idx_t r = m % 7U;

    /* prefix sums of one period of the difference pattern */
    static const seq_t prefix[8] = {
        0,  6,  8, 14, 16, 22, 26, 30
    };

    seq_t cycles = (seq_t)cycles_i;
    seq_t result = (seq_t)4;
    result += cycles * (seq_t)30;
    result += prefix[(size_t)r];
    return result;
}
