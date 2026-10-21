/*
 * A263233  Triangle read by rows: T(n,k) is the number of partitions of n
 *          having k perfect square parts (0<=k<=n).
 * https://oeis.org/A263233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattened triangle of T(r,k) where T(r,k) counts
 *            partitions of r that have k perfect square parts. The sequence
 *            is obtained by reading the triangle row‑by‑row: a(0)=T(0,0),
 *            a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 * Range:     u8 n <= 325, u16 n <= 1712, u32 n <= 9175, u64 n <= 9175,
 *            u128 n <= 9175, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Determine the row r and column k of the triangle corresponding
       to the n‑th element.  Row r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = (idx_t)((sqrt((double)(8.0 * n + 1.0)) - 1.0) / 2.0);
    /* Correct possible floating‑point rounding errors. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;          /* column index */

    /* Dynamic programming for partitions with square‑part counts. */
    size_t dim = (size_t)(r + 1);           /* number of possible sums/k counts */
    size_t size = dim * dim;
    seq_t *dp = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp) return (seq_t)0;               /* allocation failure (unlikely) */

    dp[0] = (seq_t)1;                       /* dp[0][0] = 1, empty partition */

    /* Precompute which part sizes are perfect squares. */
    unsigned char *isSq = (unsigned char *)calloc(dim, sizeof(unsigned char));
    for (idx_t s = 1; s * s <= r; ++s) isSq[s * s] = 1;

    for (idx_t part = 1; part <= r; ++part) {
        unsigned char inc = isSq[part];
        for (idx_t sum = part; sum <= r; ++sum) {
            size_t base  = sum * dim;
            size_t prev  = (sum - part) * dim;
            if (inc) {
                for (idx_t kk = inc; kk <= r; ++kk)
                    dp[base + kk] += dp[prev + kk - inc];
            } else {
                for (idx_t kk = 0; kk <= r; ++kk)
                    dp[base + kk] += dp[prev + kk];
            }
        }
    }

    seq_t result = dp[r * dim + k];
    free(dp);
    free(isSq);
    return result;
}
