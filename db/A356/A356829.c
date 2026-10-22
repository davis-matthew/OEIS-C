/*
 * A356829  Number of vertex cuts in the n-Möbius ladder.
 * https://oeis.org/A356829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 48*a(n-3) - 11*a(n-4) - 22*a(n-5)
 *            + 7*a(n-6) + 4*a(n-7) with initial values a(1)=0, a(2)=0,
 *            a(3)=8, a(4)=82, a(5)=512, a(6)=2644, a(7)=12364 (Sequences from
 *            the OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle initial values directly */
    static const seq_t init[7] = {
        (seq_t)0,  /* a(1) */
        (seq_t)0,  /* a(2) */
        (seq_t)8,  /* a(3) */
        (seq_t)82, /* a(4) */
        (seq_t)512,/* a(5) */
        (seq_t)2644,/* a(6) */
        (seq_t)12364/* a(7) */
    };

    if (n <= 7u) {
        return init[n - 1];
    }

    /* Keep the last 7 computed values to apply the recurrence */
    seq_t v[7];
    v[0] = init[0];
    v[1] = init[1];
    v[2] = init[2];
    v[3] = init[3];
    v[4] = init[4];
    v[5] = init[5];
    v[6] = init[6];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = (10 * v[6]) - (35 * v[5]) + (48 * v[4])
                     - (11 * v[3]) - (22 * v[2]) + (7 * v[1]) + (4 * v[0]);

        /* Shift values: v[0]..v[5] become v[1]..v[6], v[6] becomes next */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = v[6];
        v[6] = next;
    }
    return v[6];
}
