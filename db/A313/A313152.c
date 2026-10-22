/*
 * A313152  Coordination sequence Gal.5.112.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 4, for n >= 2: a(n) = a(n-1) + d[(n-2) mod 8],
 *            where d = {5, 5, 6, 6, 5, 5, 4, 4}. This is equivalent to: let t
 *            = n-1 (number of completed differences), a(n) = 4 +
 *            floor(t/8)*40 + sum of the first t%8 elements of d
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;

    /* Number of difference terms used up to index n: t = n-1 */
    idx_t t = n - 1;
    idx_t q = t / 8;           /* complete cycles of 8 differences */
    idx_t r = t % 8;           /* remaining differences */

    /* Prefix sums of the cycle {5,5,6,6,5,5,4,4} */
    static const seq_t prefix[9] = {
        0,  /* 0 terms            */
        5,  /* 5                  */
        10, /* 5+5                */
        16, /* 5+5+6              */
        22, /* 5+5+6+6            */
        27, /* 5+5+6+6+5          */
        32, /* 5+5+6+6+5+5        */
        36, /* +4                 */
        40  /* +4 (full cycle)    */
    };

    seq_t sum = (seq_t)q * 40 + prefix[r];
    return 4 + sum;
}
