/*
 * A341482  Number of ways to write n as an ordered sum of 5 nonprime numbers.
 * https://oeis.org/A341482
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered sums of 5 nonprime numbers
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined for n >= 5. For n < 5, a(n) = 0. */
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* cache for computed values up to max_n */
static idx_t max_n_cached = 0;   /* largest n for which cache is valid */

/* Compute the sequence up to and including n and store it in cache. */
static void recompute_cache(idx_t n)
{
    /* Sizes for temporary arrays: up to n inclusive. */
    size_t N = (size_t)n + 1;

    /* Allocate temporary DP arrays. */
    seq_t *dp_prev = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *pref    = (seq_t *)calloc(N, sizeof(seq_t));

    if (!dp_prev || !dp_curr || !pref) {
        /* Allocation failure – leave cache untouched. */
        return;
    }

    /* Base case: 0 parts */
    dp_prev[0] = 1;

    /* Sieve primes up to n.  Store in a static array. */
    size_t prime_limit = (size_t)n;
    size_t sieve_size = prime_limit + 1;
    uint8_t *is_prime = (uint8_t *)calloc(sieve_size, sizeof(uint8_t));
    if (!is_prime) {
        free(dp_prev); free(dp_curr); free(pref);
        return;
    }
    for (size_t i = 2; i < sieve_size; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= prime_limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    /* Count primes. */
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= prime_limit; ++i) {
        if (is_prime[i]) primes_cnt++;
    }
    uint64_t *primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) {
        free(dp_prev); free(dp_curr); free(pref); free(is_prime);
        return;
    }
    size_t pc = 0;
    for (size_t i = 2; i <= prime_limit; ++i) {
        if (is_prime[i]) primes[pc++] = i;
    }

    /* Five parts */
    for (size_t part = 1; part <= 5; ++part) {
        /* Build prefix sums of dp_prev */
        pref[0] = dp_prev[0];
        for (size_t i = 1; i < N; ++i) {
            pref[i] = pref[i - 1] + dp_prev[i];
        }

        /* Initial value: all compositions with any first part (1..i). */
        for (size_t i = 1; i < N; ++i) {
            dp_curr[i] = pref[i - 1];
        }
        dp_curr[0] = 0;

        /* Subtract compositions where the first part is a prime. */
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint64_t p = primes[pi];
            if (p > n) break;
            size_t up = (size_t)p;
            for (size_t i = up; i < N; ++i) {
                dp_curr[i] -= dp_prev[i - up];
            }
        }

        /* Prepare for next iteration */
        for (size_t i = 0; i < N; ++i) {
            dp_prev[i] = dp_curr[i];
            dp_curr[i] = 0;
        }
    }

    /* Allocate new cache */
    seq_t *new_cache = (seq_t *)calloc(N, sizeof(seq_t));
    if (new_cache) {
        for (size_t i = 0; i < N; ++i) {
            /* For n < 5 all terms are zero automatically. */
            new_cache[i] = dp_prev[i];
        }
        /* Replace old cache */
        free(cache);
        cache = new_cache;
        max_n_cached = n;
    }

    /* Clean up */
    free(dp_prev); free(dp_curr); free(pref);
    free(is_prime); free(primes);
}

seq_t a(idx_t n)
{
    if (n < 5) return 0;
    if (n > max_n_cached) {
        /* Recompute cache up to the requested n. */
        recompute_cache(n);
    }
    return cache[n];
}
