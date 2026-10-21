/*
 * A280912  Number of partitions of n into odd semiprimes (A046315).
 * https://oeis.org/A280912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd semiprimes (A046315)
 * Range:     u8 n <= 152, u16 n <= 333, u32 n <= 846, u64 n <= 2470,
 *            u128 n <= 8020, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* comparison routine for qsort over idx_t values */
static int cmp_idx_t(const void *aa, const void *bb)
{
    const idx_t *a = (const idx_t *)aa;
    const idx_t *b = (const idx_t *)bb;
    return (*a > *b) - (*a < *b);
}

/* Count partitions of n into odd semiprimes. */
seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate sieve array for primality testing up to n. */
    unsigned char *is_prime = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!is_prime)
        return (seq_t)0;
    memset(is_prime, 1, (size_t)(n + 1));
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= n; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= n; q += p)
                is_prime[q] = 0;

    /* Build list of odd primes up to n. */
    idx_t prime_cap = (idx_t)(n / 2 + 1);
    idx_t *primes = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }
    idx_t pcnt = 0;
    for (idx_t i = 3; i <= n; i += 2)
        if (is_prime[i])
            primes[pcnt++] = i;

    /* Generate list of odd semiprimes (p*q with p <= q). */
    idx_t semis_cap = 128;
    idx_t *semis = (idx_t *)malloc(semis_cap * sizeof(idx_t));
    if (!semis) {
        free(primes);
        free(is_prime);
        return (seq_t)0;
    }
    idx_t scnt = 0;
    for (idx_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        for (idx_t j = i; j < pcnt; ++j) {
            idx_t q = primes[j];
            idx_t prod = p * q;
            if (prod > n)
                break;
            if (scnt == semis_cap) {
                semis_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(semis, semis_cap * sizeof(idx_t));
                if (!tmp) {
                    free(semis);
                    free(primes);
                    free(is_prime);
                    return (seq_t)0;
                }
                semis = tmp;
            }
            semis[scnt++] = prod;
        }
    }

    /* No need to sort: the construction ensures ascending order. */

    /* DP array for partitions: dp[i] = #partitions of i. */
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[n + 1];
#else
    dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!dp) {
        free(semis);
        free(primes);
        free(is_prime);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = (seq_t)1;

    for (idx_t i = 0; i < scnt; ++i) {
        idx_t coin = semis[i];
        for (idx_t s = coin; s <= n; ++s)
            dp[s] += dp[s - coin];
    }

    seq_t result = dp[n];

    /* Clean up. */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    free(semis);
    free(primes);
    free(is_prime);

    return result;
}
