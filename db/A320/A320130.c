/*
 * A320130  Number of integer solutions to a^2 + b^2 + 2*c^2 + 8*d^2 = n.
 * https://oeis.org/A320130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(a,b,c,d)∈ℤ⁴ | a² + b² + 2c² + 8d² = n}
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Precompute r2[0..n] = number of integer pairs (a,b) with a² + b² = k */
    if (n == 0) {
        /* the only solution is a = b = d = c = 0 */
        return (seq_t)1;
    }

    size_t len = (size_t)n + 1;
    seq_t *r2 = (seq_t *)calloc(len, sizeof(seq_t));
    if (!r2) return (seq_t)0;    /* allocation failure */

    for (idx_t a = 0; ; ++a) {
        seq_t a2 = (seq_t)a * a;
        if (a2 > n) break;
        seq_t cntA = (a == 0) ? 1 : 2;
        for (idx_t b = 0; ; ++b) {
            seq_t b2 = (seq_t)b * b;
            seq_t idx = a2 + b2;
            if (idx > n) break;
            seq_t cntB = (b == 0) ? 1 : 2;
            r2[(size_t)idx] += cntA * cntB;
        }
    }

    seq_t total = 0;
    for (idx_t d = 0; ; ++d) {
        seq_t d2 = 8 * (seq_t)d * d;
        if (d2 > n) break;
        seq_t cntD = (d == 0) ? 1 : 2;
        seq_t rem1 = (seq_t)n - d2;
        for (idx_t c = 0; ; ++c) {
            seq_t c2 = 2 * (seq_t)c * c;
            if (c2 > rem1) break;
            seq_t cntC = (c == 0) ? 1 : 2;
            seq_t rem2 = rem1 - c2;
            total += cntD * cntC * r2[(size_t)rem2];
        }
    }

    free(r2);
    return total;
}
