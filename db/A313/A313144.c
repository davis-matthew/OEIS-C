/*
 * A313144  Coordination sequence Gal.5.219.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) - a(n-6)
 *            + a(n-7) - a(n-8) (for n > 8) with initial values a(0)=1,
 *            a(1)=4, a(2)=9, a(3)=14, a(4)=20, a(5)=25, a(6)=28, a(7)=33,
 *            a(8)=38
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values a(0)…a(8) */
    static const seq_t init[9] =
        { (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)14,
          (seq_t)20, (seq_t)25, (seq_t)28, (seq_t)33,
          (seq_t)38 };

    if (n < 9u)
        return init[(size_t)n];

    /* Buffer to hold the last nine values */
    seq_t vals[9];
    for (idx_t i = 0; i < 9; ++i)
        vals[i] = init[i];

    for (idx_t i = 9; i <= n; ++i)
    {
        /* Compute the next term using the recurrence */
        seq_t next =
            vals[8] - vals[7]
          + (seq_t)2 * vals[6]
          - (seq_t)2 * vals[5]
          + (seq_t)2 * vals[4]
          - vals[3]
          + vals[2]
          - vals[1];

        /* Shift the window left by one position */
        for (idx_t j = 0; j < 8; ++j)
            vals[j] = vals[j + 1];
        vals[8] = next;
    }

    return vals[8];
}
