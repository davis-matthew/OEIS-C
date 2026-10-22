/*
 * A315737  Coordination sequence Gal.5.90.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0 let k = ⌊n/5⌋ and r = n mod 5. a(n) = 27*k +
 *            f(r, k mod 2) where f(0,*) = 0, f(1,0) = 6, f(1,1) = 4, f(2,0) =
 *            12, f(2,1) = 9, f(3,0) = 18, f(3,1) = 15, f(4,0) = 23, f(4,1) =
 *            21
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


/* Returns the nth term of OEIS sequence A315737. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k = n / 5;
    idx_t r = n % 5;
    idx_t parity = k & 1;          /* 0 for even k, 1 for odd k */

    seq_t base = (seq_t)27 * (seq_t)k;

    static const seq_t offs[5][2] = {
        {0, 0},
        {6, 4},
        {12, 9},
        {18, 15},
        {23, 21}
    };

    seq_t offset = offs[(size_t)r][(size_t)parity];
    return base + offset;
}
