/*
 * A386939  a(n) = Sum_{k=0..n} binomial(4*n+1,k) * binomial(3*n-k-1,n-k).
 * https://oeis.org/A386939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+1,k) * binomial(3*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 directly (also works here after the product loop). */
    seq_t result = 0;

    /* B1(k) = binomial(4*n+1, k), B1(0) = 1 */
    seq_t B1 = 1;

    /* B2(0) = binomial(3*n-1, n) */
    seq_t B2 = 1;
    for (idx_t t = 1; t <= n; ++t) {
        B2 = (B2 * (seq_t)(3 * n - t)) / (seq_t)t;
    }

    for (idx_t k = 0; k <= n; ++k) {
        /* Accumulate the term */
        result += B1 * B2;

        if (k == n) break;          /* Avoid division by zero at the last step */

        /* Update B1: C(4n+1, k+1) = C(4n+1, k) * (4n+1-k) / (k+1) */
        B1 = (B1 * (seq_t)(4 * n + 1 - k)) / (seq_t)(k + 1);

        /* Update B2: C(3n-(k+1)-1, n-(k+1)) = B2 * (n-k) / (3n-k-1) */
        B2 = (B2 * (seq_t)(n - k)) / (seq_t)(3 * n - k - 1);
    }

    return result;
}
