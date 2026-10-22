/*
 * A352611  a(n) is the number of different ways to partition the set of
 *          vertices of a convex n-gon into 5 polygons.
 * https://oeis.org/A352611
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = S(n,5), where
 *   S(n,1) = 1  (for n >= 3), 0 otherwise,
 *   S(n,k) = 0 if n < 3k,
 *   S(n,k) = k*S(n-1,k) + C(n-1,2)*S(n-3,k-1)   for n >= 3k.
 * The function computes a(n) for all n >= 15 using this recurrence.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 15) return 0;  /* offset is 15, values below not required */

    /* allocate DP table: (n+1) rows, columns 0..5 */
    size_t rows = (size_t)(n + 1);
    size_t cols = 6;                 /* indices 0..5, 0 unused */
    seq_t *dp = (seq_t *)malloc(rows * cols * sizeof(seq_t));
    if (!dp) return 0;               /* allocation failure (unlikely) */

    /* zero initialize */
    for (size_t i = 0; i < rows * cols; ++i) dp[i] = 0;

    /* base: S(i,1) = 1 if i >= 3 */
    for (idx_t i = 3; i <= n; ++i) {
        dp[(size_t)i * cols + 1] = 1;
    }

    /* compute for k = 2 .. 5 */
    for (int k = 2; k <= 5; ++k) {
        seq_t K = (seq_t)k;
        for (idx_t i = 0; i <= n; ++i) {
            if (i < (idx_t)(3 * k)) {
                dp[(size_t)i * cols + k] = 0;
            } else {
                seq_t term1 = K * dp[(size_t)(i - 1) * cols + k];
                seq_t comb2 = ((seq_t)(i - 1) * (i - 2)) / 2;  /* C(i-1,2) */
                seq_t term2 = comb2 * dp[(size_t)(i - 3) * cols + (k - 1)];
                dp[(size_t)i * cols + k] = term1 + term2;
            }
        }
    }

    seq_t result = dp[(size_t)n * cols + 5];
    free(dp);
    return result;
}
