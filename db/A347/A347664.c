/*
 * A347664  Number of partitions of n into at most 6 nonprime parts.
 * https://oeis.org/A347664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n into at most 6 non‑prime parts
 *            (OEIS A347664)
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* compute a(n) with memoisation; works for any seq_t/idx_t defined by the build */
seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static idx_t      cache_max = 0;   /* highest n cached so far          */
    static seq_t *    cache     = NULL;/* array cache[n]  (0 … cache_max)  */
    static size_t     cache_cap = 0;   /* current allocation size            */

    /* If value already cached, return it */
    if (n <= cache_max && cache != NULL)
        return cache[n];

    /* Compute values up to n (inclusive) */
    size_t N = (size_t)n;              /* may truncate if idx_t > SIZE_MAX  */

    /* ------- 1.  Non‑prime sieve up to N  ------- */
    bool *is_prime = NULL;
    if (N >= 2) {
        is_prime = (bool *)calloc(N + 1, sizeof(bool));
        if (is_prime) {
            for (size_t i = 2; i <= N; ++i)
                is_prime[i] = true;
            for (size_t i = 2; i * i <= N; ++i)
                if (is_prime[i])
                    for (size_t j = i * i; j <= N; j += i)
                        is_prime[j] = false;
        }
    }

    /* ------- 2.  DP table: dp[sum * 7 + k]  ------- */
    /* k = 0 … 6  (exactly k non‑prime parts) */
    seq_t *dp = (seq_t *)calloc((N + 1) * 7, sizeof(seq_t));
    if (!dp) { free(is_prime); return 0; }

    dp[0] = 1;   /* dp[0][0] = 1 */

    for (size_t part = 1; part <= N; ++part) {
        /* skip prime parts; use only non‑prime numbers */
        if (is_prime && is_prime[part]) continue;

        for (size_t sum = part; sum <= N; ++sum) {
            size_t base = sum * 7;
            size_t prev = (sum - part) * 7;
            for (size_t k = 1; k <= 6; ++k)
                dp[base + k] += dp[prev + k - 1];
        }
    }

    /* ------- 3.  Store results into cache  ------- */
    size_t needed_cap = N + 1;
    if (needed_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) { free(is_prime); free(dp); return 0; }
        cache = tmp;
        cache_cap = needed_cap;
    }

    for (size_t sum = 0; sum <= N; ++sum) {
        seq_t total = 0;
        size_t base = sum * 7;
        for (size_t k = 0; k <= 6; ++k)
            total += dp[base + k];
        cache[sum] = total;
    }

    if (N > cache_max) cache_max = N;

    /* Clean up temporaries */
    free(is_prime);
    free(dp);

    return cache[n];
}
