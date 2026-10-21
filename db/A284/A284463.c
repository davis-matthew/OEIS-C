/*
 * A284463  Number of compositions (ordered partitions) of n into prime
 *          divisors of n.
 * https://oeis.org/A284463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into its prime divisors
 *            (ordered partitions with parts being prime divisors of n)
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 83, u64 n <= 149,
 *            u128 n <= 269, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers. The compiler will provide seq_t and idx_t types. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0)
        return (seq_t)1;   /* empty composition */
    if (n == 1)
        return (seq_t)0;   /* no prime divisors */

    /* Find distinct prime divisors of n; at most a handful for n <= 10000. */
    idx_t primes[64];
    size_t p_cnt = 0;
    idx_t temp = n;
    for (idx_t i = 2; i * i <= temp; ++i)
    {
        if (temp % i == 0)
        {
            primes[p_cnt++] = i;
            while (temp % i == 0)
                temp /= i;
        }
    }
    if (temp > 1)
        primes[p_cnt++] = temp;

    /* Dynamic programming array for compositions up to n. */
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!dp) return (seq_t)0;  /* allocation failed */

    /* Initialize dp[0] = 1, others to 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Compute dp[i] = sum_{p in primes} dp[i-p], for i = 1..n */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < p_cnt; ++j)
        {
            idx_t p = primes[j];
            if (i >= p)
                sum += dp[i - p];
        }
        dp[i] = sum;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
