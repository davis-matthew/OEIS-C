/*
 * A360606  The polygonal polynomials evaluated at x = 1/2 and normalized with
 *          2^n.
 * https://oeis.org/A360606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 + 4*n + n^2 + (n-4)*2^(n+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes a(n) from OEIS A360606 */
seq_t a(idx_t n) {
    /* Base part: n^2 + 4*n + 8 */
    seq_t base = (seq_t)n * (seq_t)n;     /* n^2 */
    base += (seq_t)4 * (seq_t)n;          /* +4*n */
    base += (seq_t)8;                    /* +8 */

    if (n >= 4) {
        /* Positive contribution (n-4)*2^(n+1) */
        seq_t part = pow2(n + 1) * (seq_t)(n - 4);
        return base + part;
    } else {
        /* Negative contribution for n < 4 */
        /* Only needed for n=0..3; values are very small */
        long long neg = (long long)(n - 4) * (long long)pow2(n + 1);
        /* neg is negative; subtract its absolute value */
        return base - (seq_t)(-neg);
    }
}
