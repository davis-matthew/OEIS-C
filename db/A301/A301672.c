/*
 * A301672  Coordination sequence for node of type V2 in "krr" 2-D tiling (or
 *          net).
 * https://oeis.org/A301672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=17, a(5)=20, a(6)=25; For
 *            n>=7, a(n)=a(n-1)-a(n-2)+2*a(n-3)-a(n-4)+a(n-5)-a(n-6)
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms for indices 0..6 */
    static const seq_t init[7] = {1, 4, 8, 13, 17, 20, 25};
    if (n < 7)
        return init[(size_t)n];

    /* ring buffer holding a(k-1), a(k-2), …, a(k-6) */
    seq_t ring[6];
    ring[0] = init[6]; /* a(6) */
    ring[1] = init[5]; /* a(5) */
    ring[2] = init[4]; /* a(4) */
    ring[3] = init[3]; /* a(3) */
    ring[4] = init[2]; /* a(2) */
    ring[5] = init[1]; /* a(1) */

    seq_t new_val = 0;
    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = a(i-1) - a(i-2) + 2*a(i-3) - a(i-4) + a(i-5) - a(i-6) */
        new_val = ring[0];
        new_val -= ring[1];
        new_val += (seq_t)2 * ring[2];
        new_val -= ring[3];
        new_val += ring[4];
        new_val -= ring[5];

        /* shift ring: drop oldest, shift others right */
        ring[5] = ring[4];
        ring[4] = ring[3];
        ring[3] = ring[2];
        ring[2] = ring[1];
        ring[1] = ring[0];
        ring[0] = new_val;
    }
    return new_val;
}
