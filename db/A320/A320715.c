/*
 * A320715  Indices of primes followed by a gap (distance to next larger
 *          prime) of 34.
 * https://oeis.org/A320715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i such that prime(i+1) = prime(i) + 34 (indices start at
 *            1)
 * Range:     u8 n <= 1, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------
 * Sieve of Eratosthenes – computes all primes <= limit.
 * The array of primes is returned in primes_out (caller must free it);
 * primes_cnt holds the number of primes found.
 * -------------------------------------------------------------------- */
static void sieve_primes_upto(uint64_t limit, uint64_t **primes_out, size_t *primes_cnt)
{
    size_t sz = (size_t)(limit + 1);                      /* indices 0 .. limit */
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) {
        *primes_out = NULL;
        *primes_cnt = 0;
        return;
    }

    /* initialise all entries to true */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Eratosthenes */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint64_t p = 2; p <= limit; ++p)
        if (is_prime[p]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t p = 2; p <= limit; ++p)
        if (is_prime[p]) primes[idx++] = p;

    free(is_prime);
    *primes_out = primes;
    *primes_cnt = cnt;
}

/* --------------------------------------------------------------------
 * Static cache for the sequence values.
 * -------------------------------------------------------------------- */
static uint64_t *g_indices = NULL;   /* indices of primes with gap 34, 1‑based */
static size_t    g_len      = 0;     /* number of stored terms              */
static uint64_t  g_limit    = 0;     /* current sieve upper bound            */

/* --------------------------------------------------------------------
 * Public function: returns a(n) for n>=1.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 – undefined for 0 */

    /* If enough terms are already cached, return the requested one. */
    if (g_len >= (size_t)n)
        return (seq_t)g_indices[(size_t)n - 1];

    /* Start with a reasonable limit or use the current one. */
    uint64_t limit = g_limit ? g_limit : 100000ULL;

    /* Grow the sieve until we have accumulated at least n indices. */
    while (g_len < (size_t)n) {
        /* Double the limit (and add a small buffer to avoid zero). */
        limit <<= 1;
        if (limit == 0) limit = UINT64_MAX / 2;

        /* Compute all primes up to the new limit. */
        uint64_t *primes = NULL;
        size_t    primes_cnt = 0;
        sieve_primes_upto(limit, &primes, &primes_cnt);
        if (primes_cnt == 0) /* allocation failed */ {
            free(primes);
            break;
        }

        /* Identify indices of primes followed by a gap of 34. */
        uint64_t *tmp = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
        size_t    cnt = 0;
        for (size_t i = 1; i < primes_cnt; ++i) {
            if (primes[i] - primes[i - 1] == 34)
                tmp[cnt++] = (uint64_t)i;   /* 1‑based index of the lower prime */
        }

        /* Replace the cached indices with the newly computed ones. */
        uint64_t *newidx = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        for (size_t j = 0; j < cnt; ++j)
            newidx[j] = tmp[j];

        free(tmp);
        free(primes);
        free(g_indices);

        g_indices = newidx;
        g_len     = cnt;
        g_limit   = limit;
    }

    /* If we still lack enough terms, return 0 (should not happen for valid input). */
    if (g_len >= (size_t)n)
        return (seq_t)g_indices[(size_t)n - 1];
    return 0;
}
