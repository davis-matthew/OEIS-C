/*
 * A398826  Number of partitions of n where the largest part is equal to the
 *          number of divisors of n.
 * https://oeis.org/A398826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n, τ(n)) – p(n, τ(n)–1) where τ(n) is the number of
 *            divisors of n and p(n, k) is the number of partitions of n with
 *            parts ≤ k
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 179, u64 n <= 599,
 *            u128 n <= 2519, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"

/* external function to be implemented elsewhere */
seq_t a(idx_t n)
{
    if (n == 0)          /* should not happen for n ≥ 1, but keep safe */
        return (seq_t)0;

    size_t N = (size_t)n;
    size_t d = (size_t)divisor_count_u64(n);  /* τ(n) */

    /* compute p(N, d) */
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;            /* out of memory – unlikely for test size */

    dp[0] = (seq_t)1;
    for (size_t k = 1; k <= d; ++k) {
        for (size_t i = k; i <= N; ++i) {
            dp[i] += dp[i - k];
        }
    }
    seq_t with_max = dp[N];
    free(dp);

    /* compute p(N, d-1) only if d>1 */
    seq_t without_max = (seq_t)0;
    if (d > 1) {
        seq_t *dp2 = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (!dp2)
            return with_max;         /* fallback – should not happen */

        dp2[0] = (seq_t)1;
        for (size_t k = 1; k <= d - 1; ++k) {
            for (size_t i = k; i <= N; ++i) {
                dp2[i] += dp2[i - k];
            }
        }
        without_max = dp2[N];
        free(dp2);
    }

    return with_max - without_max;
}
