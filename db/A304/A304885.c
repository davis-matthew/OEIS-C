/*
 * A304885  Expansion of Product_{k>=1} 1/(1-x^(3*k-2)) * Product_{k>=1}
 *          1/(1-x^(6*k-1)).
 * https://oeis.org/A304885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 1 mod 3
 *            * or 5 mod 6 (i.e. allowed part sizes
 *            1,4,5,7,10,11,13,16,17,19,…). * Recurrence: a(0)=1; for n>0, *
 *            a(n)=∑_{m∈S, m≤n} a(n‑m) where S={m | m%3==1 or m%6==5}
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t size = n + 1;
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return (seq_t)0;           /* allocation failure -> 0 */

    for (idx_t i = 0; i < size; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        if ((m % 3 == 1) || (m % 6 == 5)) {  /* allowed part size */
            for (idx_t x = m; x <= n; ++x) {
                dp[x] += dp[x - m];
            }
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
