/*
 * A330174  Number of primitive Pythagorean triangles with sum of legs n.
 * https://oeis.org/A330174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primitive Pythagorean triples with sum of legs
 *            n Computed by enumerating Euclid's parameters (m,n) and counting
 *            solutions a + b = n
 * Range:     u8 n <= 3958, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t N = (seq_t)n;
    seq_t count = 0;

    for (idx_t m = 2; ; ++m) {
        seq_t m_sq   = (seq_t)m * (seq_t)m;
        seq_t min_sum = m_sq + 2 * (seq_t)m - 1;   /* sum when small n = 1 */
        if (min_sum > N) break;                    /* no further m can work */

        for (idx_t ns = 1; ns < m; ++ns) {
            /* require opposite parity */
            if (((m ^ ns) & 1u) == 0) continue;

            /* require coprime */
            if (gcd((seq_t)m, (seq_t)ns) != 1) continue;

            seq_t a_leg = m_sq - (seq_t)ns * (seq_t)ns;
            seq_t b_leg = 2 * (seq_t)m * (seq_t)ns;
            seq_t sum = a_leg + b_leg;

            if (sum == N) ++count;
        }
    }

    return count;
}
