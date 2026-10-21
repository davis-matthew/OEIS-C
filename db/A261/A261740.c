/*
 * A261740  Number of partitions of n where each part i is marked with a word
 *          of length i over a senary alphabet whose letters appear in
 *          alphabetical order.
 * https://oeis.org/A261740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==0, otherwise a(n) = sum_{k=1}^{n} binomial(k+5,5)
 *            * a(n-k) where binomial(k+5,5) = (k+1)(k+2)(k+3)(k+4)(k+5)/120
 *            The generating function is Π_{k≥1} 1/(1 - binomial(k+5,5)·x^k)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute a(n) – weighted partition number */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* allocate dp[0..n] */
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!dp)
        return (seq_t)0;               /* allocation failed */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* constant 5! = 120 */
    const seq_t denom = (seq_t)120;

    /* dynamic programming over part sizes k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* binomial(k+5,5) = (k+1)(k+2)(k+3)(k+4)(k+5)/120 */
        seq_t w = (seq_t)(k + 1);
        w *= (seq_t)(k + 2);
        w *= (seq_t)(k + 3);
        w *= (seq_t)(k + 4);
        w *= (seq_t)(k + 5);
        w /= denom;

        /* update dp[s] for all s >= k */
        for (idx_t s = k; s <= n; ++s) {
            dp[s] += dp[s - k] * w;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
