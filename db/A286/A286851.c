/*
 * A286851  Number of compositions (ordered partitions) of n into unitary
 *          divisors of n.
 * https://oeis.org/A286851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into unitary divisors of n
 *            Computation: for each n, build the set of unitary divisors S(n)
 *            and compute dp[0..n] where dp[s] = ∑_{d∈S, d≤s} dp[s-d]. dp[0] =
 *            1; the value a(n) = dp[n]
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 41, u64 n <= 77, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* Static cache of already computed a(n) values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Static DP buffer reused for each computation */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;

    /*--- Ensure cache is large enough ---*/
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_cap = 1;
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
    } else if ((size_t)n >= cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        for (size_t i = cache_cap; i <= (size_t)n; ++i)
            cache[i] = (seq_t)0;
        cache_cap = n + 1;
    }

    /* Return cached value if already computed */
    if (cache[n] != (seq_t)0 || n == 0)
        return cache[n];

    /*--- Ensure DP buffer is large enough ---*/
    if ((size_t)n + 1 > dp_cap) {
        size_t new_cap = (dp_cap == 0) ? (n + 1) : dp_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        dp = tmp;
        dp_cap = new_cap;
    }

    /* Initialize DP */
    dp[0] = (seq_t)1;
    for (idx_t s = 1; s <= n; ++s)
        dp[s] = (seq_t)0;

    /* Factor n to obtain its prime powers (for unitary divisors) */
    idx_t tmp = n;
    idx_t primes[10];
    idx_t exps[10];
    size_t pcnt = 0;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Compute prime^exponent values */
    seq_t prime_pow[10];
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t val = (seq_t)1;
        for (idx_t j = 0; j < exps[i]; ++j)
            val *= (seq_t)primes[i];
        prime_pow[i] = val;
    }

    /* Generate all unitary divisors (each prime either used in full power or not) */
    seq_t divisors[64];
    size_t dcnt = 0;
    size_t total_masks = ((size_t)1 << pcnt);
    for (size_t mask = 0; mask < total_masks; ++mask) {
        seq_t d = (seq_t)1;
        for (size_t bit = 0; bit < pcnt; ++bit)
            if (mask & ((size_t)1 << bit))
                d *= prime_pow[bit];
        divisors[dcnt++] = d;
    }

    /* Dynamic programming to count compositions */
    for (idx_t sum = 1; sum <= n; ++sum) {
        seq_t tot = (seq_t)0;
        for (size_t k = 0; k < dcnt; ++k) {
            seq_t d = divisors[k];
            if (d <= (seq_t)sum)
                tot += dp[sum - (idx_t)d];
        }
        dp[sum] = tot;
    }

    cache[n] = dp[n];
    return dp[n];
}
