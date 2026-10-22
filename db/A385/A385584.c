/*
 * A385584  a(n) is the number of pairs (p, t) such that p is a pyramidal
 *          number, t is a triangular number, p + t <= n and t <= p.
 * https://oeis.org/A385584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = card({(p in A000292, t in A000217, t <= p, p + t <= n})
 * Range:     u8 n <= 17, u16 n <= 4289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count pairs (pyramidal number, triangular number) satisfying the constraints */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t i = 0; ; ++i) {
        seq_t pi = (seq_t)i * (i + 1) * (i + 2) / 6;   /* pyramidal number p_i */
        if (pi > n) break;                            /* no larger pyramidal numbers can be used */

        seq_t L = pi;                                 /* upper limit for t */
        seq_t rem = n - pi;
        if (rem < L) L = rem;                         /* t must also satisfy p + t <= n */

        /* find largest j such that j*(j+1)/2 <= L */
        seq_t val = 1 + (seq_t)8 * L;
        seq_t s   = floor_sqrt(val);                  /* floor(sqrt(1 + 8*L)) */
        seq_t jmax = (s - 1) / 2;                     /* largest j */

        total += jmax + 1;                            /* include j = 0 (t = 0) */
    }
    return total;
}
