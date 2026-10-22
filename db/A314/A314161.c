/*
 * A314161  Coordination sequence Gal.4.140.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) - a(n-6) for n > 6;
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=16, a(4)=22, a(5)=28, a(6)=33;
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 6 */
    static const seq_t base[7] = {
        (seq_t)1, (seq_t)5, (seq_t)11,
        (seq_t)16, (seq_t)22, (seq_t)28,
        (seq_t)33
    };
    if (n <= 6)
        return base[n];

    /* Sliding window holding the last 7 computed values */
    seq_t last[7];
    for (idx_t k = 0; k < 7; ++k)
        last[k] = base[k];

    for (idx_t i = 7; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) - a(i-2) - a(i-4) + 2*a(i-5) - a(i-6) */
        seq_t next = 2 * last[6]
                   - last[5]
                   - last[3]
                   + 2 * last[2]
                   - last[1];

        /* Shift window: discard a(i-6), shift others left, insert a(i) */
        for (idx_t j = 0; j < 6; ++j)
            last[j] = last[j + 1];
        last[6] = next;
    }

    return last[6];
}
