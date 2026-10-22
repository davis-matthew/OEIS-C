/*
 * A320138  Number of integer solutions to a^2 + 2*b^2 + 3*c^2 + 3*d^2 = n.
 * https://oeis.org/A320138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of integer solutions to a^2 + 2*b^2 + 3*c^2 +
 *            3*d^2 = n (a, b, c, d ∈ ℤ)
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate tables for the two quadratic forms */
    size_t sz = (size_t)(n + 1);
    seq_t *AB = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *CD = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!AB || !CD) {
        free(AB);
        free(CD);
        return (seq_t)0;   /* Allocation failure */
    }

    /* Count representations of a^2 + 2*b^2 */
    for (idx_t a = 0; ; ++a) {
        idx_t a2 = a * a;
        if (a2 > n) break;                /* a^2 alone already exceeds n */
        for (idx_t b = 0; ; ++b) {
            idx_t b2 = b * b;
            idx_t val = a2 + 2 * b2;
            if (val > n) break;           /* further b will only increase val */
            seq_t mult = 1;
            if (a != 0) mult *= 2;
            if (b != 0) mult *= 2;
            AB[(size_t)val] += mult;
        }
    }

    /* Count representations of 3*(c^2 + d^2) */
    for (idx_t c = 0; ; ++c) {
        idx_t c2 = c * c;
        if (3 * c2 > n) break;            /* even d = 0 would exceed n */
        for (idx_t d = 0; ; ++d) {
            idx_t d2 = d * d;
            idx_t val = 3 * (c2 + d2);
            if (val > n) break;           /* further d increases val */
            seq_t mult = 1;
            if (c != 0) mult *= 2;
            if (d != 0) mult *= 2;
            CD[(size_t)val] += mult;
        }
    }

    /* Convolution: total number of quadruples summing to n */
    seq_t result = 0;
    for (idx_t m = 0; m <= n; ++m) {
        seq_t r1 = AB[(size_t)n - m];
        seq_t r2 = CD[(size_t)m];
        unsigned __int128 prod = (unsigned __int128)r1 * (unsigned __int128)r2;
        result += (seq_t)prod;             /* truncate to seq_t if necessary */
    }

    free(AB);
    free(CD);
    return result;
}
