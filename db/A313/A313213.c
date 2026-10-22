/*
 * A313213  Coordination sequence Gal.6.260.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 4 + 5*(n-1) - 2*floor((n-1)/12) +
 *            p[(n-1) % 12], where p = {0,0,1,0,0,0,0,0,-1,0,0,-1}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* m = n - 1, q = floor((n-1)/12), r = (n-1) % 12 */
    idx_t m = n - 1;
    idx_t q = m / (idx_t)12;
    idx_t r = m % (idx_t)12;

    static const int p[12] = {0, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, -1};

    /* Compute in seq_t to avoid intermediate overflows
       (seq_t is an unsigned integer type large enough for the result). */
    seq_t result = (seq_t)4
                 + (seq_t)5 * (seq_t)m
                 - (seq_t)2 * (seq_t)q
                 + (seq_t)p[(int)r];

    return result;
}
