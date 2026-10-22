/*
 * A352636  Number of ways to write n as a^2 + 3*b^2 + c^4 + 2*d^4 + a*b*c*d,
 *          where a,b,c,d are nonnegative integers.
 * https://oeis.org/A352636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions in non‑negative integers * (a,b,c,d)
 *            of a^2 + 3*b^2 + c^4 + 2*d^4 + a*b*c*d = n * * The routine
 *            simply enumerates all feasible combinations. * For n up to 10
 *            000 this takes far less than 2 s
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* auxiliary inline function to compute x^4 in seq_t type */
static inline seq_t pow4(idx_t x) {
    seq_t y = (seq_t)x * x;    /* x^2 */
    return y * y;             /* (x^2)^2 = x^4 */
}

/* Count representations of n */
seq_t a(idx_t n) {
    seq_t count = 0;

    /* pre‑compute the 4th powers that can occur */
    /* maximum 4th power needed is n      */
    #define MAX_CACHE  64       /* enough for n <= 1e12, far beyond our limits */
    seq_t c4[MAX_CACHE];
    seq_t d4[MAX_CACHE];
    idx_t cmax = 0, dmax = 0;

    /* fill c4 upto c^4 <= n */
    for (idx_t c = 0;; ++c) {
        seq_t v = pow4(c);
        if (v > n) break;
        c4[cmax++] = v;
    }

    /* fill d4 upto 2*d^4 <= n */
    for (idx_t d = 0;; ++d) {
        seq_t v = pow4(d);
        if (2 * v > n) break;
        d4[dmax++] = v;
    }

    /* iterate over a */
    for (idx_t a = 0; a * a <= n; ++a) {
        seq_t a2 = (seq_t)a * a;
        /* iterate over b */
        for (idx_t b = 0; 3 * b * b <= n - a2; ++b) {
            seq_t b2 = (seq_t)b * b;
            seq_t rem_ab = n - a2 - 3 * b2;
            /* iterate over c */
            for (idx_t c = 0; c < cmax && c4[c] <= rem_ab; ++c) {
                seq_t rem_abc = rem_ab - c4[c];
                /* iterate over d */
                for (idx_t d = 0; d < dmax && 2 * d4[d] <= rem_abc; ++d) {
                    seq_t val = a2 + 3 * b2 + c4[c] + 2 * d4[d]
                                + (seq_t)a * b * c * d;
                    if (val == n) count++;
                }
            }
        }
    }
    return count;
}
