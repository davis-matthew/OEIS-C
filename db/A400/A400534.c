/*
 * A400534  a(n) is the maximum number of entries in row n of Pascal's
 *          triangle divisible by the same prime.
 * https://oeis.org/A400534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = n + 1 - min_{p prime, p <= n} ∏_i (n_i(p) + 1) where
 *            n_i(p) are the base‑p digits of n. For n < 2, a(n) = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- prime cache ------------------------------------------------------- */
static uint64_t *prime_cache = NULL;   /* array of primes up to current limit */
static idx_t     prime_cache_limit = 0;/* inclusive maximum n having primes cached */
static size_t   prime_cache_cnt = 0;   /* number of primes stored in prime_cache */

/* rebuild primes up to (and including) n, storing them in prime_cache */
static void rebuild_primes_upto(idx_t n)
{
    /* simple sieve up to n */
    size_t len = (size_t)n + 1;
    bool *is_comp = (bool *)malloc(len);
    if (!is_comp)
        return; /* allocation failure – keep old cache */
    for (size_t i = 0; i < len; ++i)
        is_comp[i] = false;

    for (size_t i = 2; i * i <= (size_t)n; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_comp[j] = true;

    /* count primes to know allocation size */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (!is_comp[i])
            ++cnt;

    /* resize cache if necessary */
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, cnt * sizeof(uint64_t));
    if (!tmp && cnt > 0) {
        free(is_comp);
        return; /* keep old cache on failure */
    }
    prime_cache = tmp;
    prime_cache_cnt = cnt;

    /* fill cache with primes */
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (!is_comp[i])
            prime_cache[idx++] = (uint64_t)i;

    prime_cache_limit = n;
    free(is_comp);
}

/* --- sequence ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* offset 0: a(0)=0, a(1)=0 */
    if (n < 2)
        return 0;

    /* extend prime cache if needed */
    if (n > prime_cache_limit)
        rebuild_primes_upto(n);

    /* minimal product over all primes <= n */
    uint64_t min_prod = (uint64_t)(n + 1); /* largest possible product */

    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        idx_t p = (idx_t)prime_cache[i];
        if (p > n)
            break; /* cache primes are sorted */

        idx_t tmp = n;
        uint64_t prod = 1;

        while (tmp) {
            idx_t digit = tmp % p;
            prod *= (uint64_t)(digit + 1);
            if (prod >= min_prod)
                break;          /* no need to continue */
            tmp /= p;
        }

        if (prod < min_prod) {
            min_prod = prod;
            if (min_prod == 2)      /* cannot get smaller than 2 */
                break;
        }
    }

    return (seq_t)(n + 1) - (seq_t)min_prod;
}
