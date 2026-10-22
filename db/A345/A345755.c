/*
 * A345755  a(n) is the number of primes p satisfying n*(log_2(n))^2 < p <=
 *          (n+1)*(log_2(n+1))^2.
 * https://oeis.org/A345755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p : n·(log₂ n)² < p ≤ (n+1)·(log₂(n+1))² }
 *            (offset 1)
 * Range:     u8 n <= 2772, u16 n <= 2772, u32 n <= 2772, u64 n <= 2772,
 *            u128 n <= 2772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Prime counting cache --------------------------------------------------- */
static idx_t *pi_cache = NULL;      /* cumulative counts of primes, Π(x) for 0 ≤ x ≤ pi_cache_limit */
static idx_t pi_cache_limit = 0;    /* current maximum index cached */

/* Extend prime cache up to at least 'limit' */
static void extend_prime_cache(idx_t limit)
{
    if (limit <= pi_cache_limit)
        return;

    /* Allocate sieve array */
    bool *is_comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp)
        return;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Build cumulative prime count array */
    idx_t *new_cache = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!new_cache) {
        free(is_comp);
        return;
    }

    idx_t cnt = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (i >= 2 && !is_comp[i])
            ++cnt;
        new_cache[i] = cnt;
    }

    /* Replace old cache */
    free(pi_cache);
    pi_cache = new_cache;
    pi_cache_limit = limit;
    free(is_comp);
}

/* Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* undefined, but return 0 as a safe value */

    /* Compute bounds using double arithmetic */
    double log2n   = log2((double)n);
    double lower   = (double)n * log2n * log2n;

    double log2np1 = log2((double)(n + 1));
    double upper   = (double)(n + 1) * log2np1 * log2np1;

    /* Floor to integer indices */
    idx_t L = (idx_t)floor(lower);
    idx_t U = (idx_t)floor(upper);

    if (U < 2)          /* no prime can lie in this interval */
        return (seq_t)0;

    if (L > U)          /* empty interval (theoretically impossible here) */
        return (seq_t)0;

    /* Ensure cache covers the needed upper bound */
    extend_prime_cache(U);

    /* Count primes > L and ≤ U using cumulative counts */
    seq_t count = (seq_t)pi_cache[U];
    if (L <= U)
        count -= (seq_t)pi_cache[L];

    return count;
}
