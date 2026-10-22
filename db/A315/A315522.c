/*
 * A315522  Coordination sequence Gal.6.621.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-6) + 2*a(n-7) - a(n-8) for n > 8
 *            Initial terms: a(0)=1, a(1)=6, a(2)=11, a(3)=17, a(4)=22,
 *            a(5)=28, a(6)=33, a(7)=38, a(8)=44
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const seq_t INIT[9] = {
    (seq_t)1,  (seq_t)6,  (seq_t)11,
    (seq_t)17, (seq_t)22, (seq_t)28,
    (seq_t)33, (seq_t)38, (seq_t)44
};

seq_t a(idx_t n)
{
    /* Return the pre‑computed value for indices 0..8. */
    if (n < 9) {
        return INIT[(size_t)n];
    }

    /* Circular buffer holding the last 9 sequence values:
         buf[0] = a(k-8), buf[1] = a(k-7), … , buf[8] = a(k-1)
       Initially it contains a(0) … a(8). */
    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i) {
        buf[i] = INIT[i];
    }

    /* Compute terms from k = 9 up to n. */
    for (idx_t k = 9; k <= n; ++k) {
        /* a(k) = 2*a(k-1) - a(k-2) - a(k-6) + 2*a(k-7) - a(k-8)  */
        seq_t val = 2 * buf[8] - buf[7] - buf[3] + 2 * buf[2] - buf[1];

        /* Shift the buffer left by one position. */
        for (size_t i = 0; i < 8; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[8] = val;
    }

    return buf[8];  /* a(n) */
}
