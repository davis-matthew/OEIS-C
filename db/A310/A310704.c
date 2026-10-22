/*
 * A310704  Coordination sequence Gal.3.2.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4) - a(n-5) + a(n-6) -
 *            a(n-7) * Initial values: a(0)=1, a(1)=4, a(2)=7, a(3)=10,
 *            a(4)=15, a(5)=19, * a(6)=21, a(7)=24
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)4, (seq_t)7, (seq_t)10,
        (seq_t)15, (seq_t)19, (seq_t)21, (seq_t)24
    };
    if (n < 8) return init[n];

    /* buffer holds the last eight values: buf[0] = a(m-7), ..., buf[7] = a(m-1) */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = init[i];

    seq_t cur = 0;
    for (size_t k = 8; k <= n; ++k) {
        /* compute a(k) from a(k-1) ... a(k-7) */
        cur = buf[7] - buf[6] + buf[5] + buf[4] - buf[3] + buf[2] - buf[1];

        /* shift buffer to discard the oldest value */
        for (int i = 0; i < 7; ++i) buf[i] = buf[i + 1];
        buf[7] = cur;
    }
    return cur;
}
