/*
 * A313297  Coordination sequence Gal.6.210.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..10) given. Recurrence for n>10: a(n) = 3*a(n-1) - 4*a(n-2)
 *            + 3*a(n-3) - a(n-4) - a(n-6) + 3*a(n-7) - 4*a(n-8) + 3*a(n-9) -
 *            a(n-10) (Initial values a(0)=1, a(1)=4, a(2)=9, a(3)=15,
 *            a(4)=21, a(5)=27, a(6)=33, a(7)=39, a(8)=45, a(9)=51, a(10)=57)
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t init[11] = {1,4,9,15,21,27,33,39,45,51,57};
    if (n < 11) return init[n];

    seq_t a0 = init[0]; /* a(n-10) */
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    seq_t a7 = init[7];
    seq_t a8 = init[8];
    seq_t a9 = init[9];
    seq_t a10 = init[10]; /* a(n-1) when i = 11 */

    seq_t val = a10; /* will hold a(n) after loop */
    for (idx_t i = 11; i <= n; ++i) {
        val = 3*a10 - 4*a9 + 3*a8 - a7 - a5 + 3*a4 - 4*a3 + 3*a2 - a1;
        /* shift the sliding window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = a9;
        a9 = a10;
        a10 = val;
    }
    return val;
}
