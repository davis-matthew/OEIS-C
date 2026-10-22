/*
 * A314685  Coordination sequence Gal.3.16.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=9, a(3)=13, a(4)=17; For n>=5: a(n)=22 +
 *            22*floor((n-5)/5) + {0,5,9,13,17}[(n-5)%5]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms */
    if (n < 5) {
        static const seq_t base[5] = {1, 5, 9, 13, 17};
        return base[(size_t)n];
    }

    /* n >= 5 */
    idx_t m      = n - 5;                /* steps beyond a(5) */
    idx_t cycles = m / 5;                /* number of full 5‑step cycles */
    idx_t rem    = m % 5;                /* remaining steps in the current cycle */

    static const seq_t prefix[6] = {0, 5, 9, 13, 17, 22};
    seq_t sum = (seq_t)22 * (seq_t)(cycles + 1) + prefix[(size_t)rem];
    return sum;
}
