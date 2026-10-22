/*
 * A337901  The number of walks of length 2n+1 on the square lattice that
 *          start from the origin (0,0) and end at the vertex (3,0).
 * https://oeis.org/A337901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = [n/(n+2) * binom(2n+1, n)]^2 * where binom(2n+1, n) =
 *            product_{k=1..n} (n+k+1)/k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t binom = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* multiply by (n + k + 1) and divide by k, division is exact */
        seq_t num  = (seq_t)(n + k + 1);
        binom = binom * num / (seq_t)k;
    }

    /* compute A002054(n) = binom * n / (n + 2) */
    seq_t val = binom * (seq_t)n;
    val = val / (seq_t)(n + 2);

    /* square the result */
    return val * val;
}
