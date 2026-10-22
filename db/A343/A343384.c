/*
 * A343384  Number of ways to write n as [a^3/3] + [b^3/4] + [c^3/5] + [d^6/6]
 *          with a,b,c,d positive integers, where [.] is the floor function.
 * https://oeis.org/A343384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integer quadruples (a,b,c,d) such that
 *            n = ⌊a³/3⌋ + ⌊b³/4⌋ + ⌊c³/5⌋ + ⌊d⁶/6⌋
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case n = 0: only the quadruple (1,1,1,1) works. */
    if (n == 0) return 1;

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t a = 1; ; ++a) {
        seq_t a3 = (seq_t)a * a * a;
        seq_t va  = a3 / 3;
        if (va > n_seq) break;

        for (idx_t b = 1; ; ++b) {
            seq_t b3 = (seq_t)b * b * b;
            seq_t vb  = b3 / 4;
            if (vb > n_seq) break;

            for (idx_t c = 1; ; ++c) {
                seq_t c3 = (seq_t)c * c * c;
                seq_t vc  = c3 / 5;
                if (vc > n_seq) break;

                for (idx_t d = 1; ; ++d) {
                    /* Compute d⁶ as a 64‑bit value. */
                    unsigned long long d6 = 1;
                    for (int k = 0; k < 6; ++k) d6 *= d;

                    seq_t vd = (seq_t)(d6 / 6ULL);
                    if (vd > n_seq) break;            /* sum will only grow larger */
                    seq_t sum = va + vb + vc + vd;
                    if (sum > n_seq) break;           /* further d will increase vd */
                    if (sum == n_seq) ++count;        /* found a valid quadruple */
                }
            }
        }
    }
    return count;
}
