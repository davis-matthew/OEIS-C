/*
 * A350459  Number of positive rational points on the unit circle with
 *          denominator <= n and numerator >= 1.
 * https://oeis.org/A350459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (a,b,c,d) with 1 ≤ c,d ≤ n, 1 ≤ b ≤
 *            a, a^2/c^2 + b^2/d^2 = 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t total = 0;

    /* Traverse primitive Pythagorean triples (a0,b0,c0) with c0 ≤ n.
       Each triple gives two orientations:
          orientation 1:  b = k*b0,  e = k*a0,  d = k*c0
          orientation 2:  b = k*a0,  e = k*b0,  d = k*c0
       For every orientation we count suitable a. */
    for (idx_t m = 2; m * m <= n; ++m) {
        for (idx_t p = 1; p < m; ++p) {
            if (((m - p) & 1) == 0) continue;          /* opposite parity */
            if (gcd_idx(m, p) != 1) continue;         /* coprime */

            idx_t a0 = m * m - p * p;                 /* legs of primitive triple */
            idx_t b0 = 2 * m * p;
            idx_t c0 = m * m + p * p;
            if (c0 > n) continue;

            /* Orientation 1 */
            for (idx_t k = 1, dk = c0; dk <= n; ++k, dk = k * c0) {
                idx_t b = b0 * k;
                idx_t e = a0 * k;          /* e > 0 because a0 > 0 */
                idx_t d = dk;

                idx_t g = gcd_idx(e, d);
                idx_t e1 = e / g;        /* e1 > 0 */

                /* first a ≥ b that is multiple of e1 */
                idx_t first;
                if (b % e1 == 0)
                    first = b;
                else
                    first = ((b + e1 - 1) / e1) * e1;

                /* maximum a so that c = a*d/e ≤ n */
                idx_t ul = (n * e) / d;   /* safe: n,e,d ≤ 10000 */

                if (first > ul) continue;

                /* number of a in arithmetic progression */
                seq_t cnt = (seq_t)((ul - first) / e1) + 1;
                total += cnt;
            }

            /* Orientation 2 */
            for (idx_t k = 1, dk = c0; dk <= n; ++k, dk = k * c0) {
                idx_t b = a0 * k;
                idx_t e = b0 * k;          /* e > 0 */
                idx_t d = dk;

                idx_t g = gcd_idx(e, d);
                idx_t e1 = e / g;          /* e1 > 0 */

                idx_t first;
                if (b % e1 == 0)
                    first = b;
                else
                    first = ((b + e1 - 1) / e1) * e1;

                idx_t ul = (n * e) / d;

                if (first > ul) continue;

                seq_t cnt = (seq_t)((ul - first) / e1) + 1;
                total += cnt;
            }
        }
    }
    return total;
}
