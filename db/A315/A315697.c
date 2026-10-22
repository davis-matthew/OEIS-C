/*
 * A315697  Coordination sequence Gal.6.621.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-6) + 2*a(n-7) - a(n-8) for n > 8
 *            with the initial values a(0)=1, a(1)=6, a(2)=12, a(3)=17,
 *            a(4)=23, a(5)=28, a(6)=33, a(7)=38, a(8)=43
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[9] = {1, 6, 12, 17, 23, 28, 33, 38, 43};

seq_t a(idx_t n)
{
    if (n < 9)
        return init[n];

    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i)
        buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        idx_t i1 = (i - 1) % 9;
        idx_t i2 = (i - 2) % 9;
        idx_t i6 = (i - 6) % 9;
        idx_t i7 = (i - 7) % 9;
        idx_t i8 = (i - 8) % 9;

        seq_t next = 2 * buf[i1] - buf[i2] - buf[i6] + 2 * buf[i7] - buf[i8];
        buf[i % 9] = next;
    }

    return buf[n % 9];
}
