/*
 * A389396  Numbers k such that binomial(2*k,k) is divisible by
 *          (k+1)^2*(k+2)^2.
 * https://oeis.org/A389396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 576, u32 n <= 1468, u64 n <= 1468,
 *            u128 n <= 1468, big any n
 * Helpers:   v_binom_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The sequence of integers k such that
 * binomial(2*k,k) is divisible by (k+1)^2*(k+2)^2.
 * a(1) = 208, a(2) = 458, …
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/v_binom_prime.h"

/* --------------------------------------------------------------------- */
/* Prime sieve – static cache of primes up to a fixed limit.             */
/* --------------------------------------------------------------------- */
static bool primes_initialized = false;
static uint32_t *primes = NULL;
static size_t primes_count = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    const idx_t LIMIT = 1000000;                 /* sufficient for usual n ≤ 10 000 */
    char *is_comp = (char *)calloc((size_t)(LIMIT + 1U), 1);
    if (!is_comp)
        exit(EXIT_FAILURE);

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (!is_comp[p]) {
            for (idx_t q = p * p; q <= LIMIT; q += p)
                is_comp[q] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes)
        exit(EXIT_FAILURE);

    primes_count = cnt;
    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            primes[pos++] = (uint32_t)i;

    free(is_comp);
    primes_initialized = true;
}

/* --------------------------------------------------------------------- */
/* Utility: exponent of prime p in binomial(2*k,k) via Legendre formula  */

/* --------------------------------------------------------------------- */
/* Factorise n into arrays of primes and exponents (up to 32 factors).   */
/* --------------------------------------------------------------------- */
static void factorize(idx_t n, uint32_t *pf, uint8_t *ep, size_t *len_out)
{
    size_t len = 0;

    for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * primes[i] <= (uint64_t)n; ++i) {
        uint32_t pr = primes[i];
        if (n % pr == 0) {
            uint8_t e = 0;
            while (n % pr == 0) {
                n /= pr;
                ++e;
            }
            pf[len] = pr;
            ep[len] = e;
            ++len;
        }
    }
    if (n > 1) {
        pf[len] = (uint32_t)n;
        ep[len] = 1;
        ++len;
    }
    *len_out = len;
}

/* --------------------------------------------------------------------- */
/* Check whether a particular k satisfies the required divisibility    */
/* --------------------------------------------------------------------- */
static bool satisfies(idx_t k)
{
    uint32_t pf[32];
    uint8_t ep[32];
    size_t len;

    /* check (k+1) */
    factorize(k + 1, pf, ep, &len);
    for (size_t i = 0; i < len; ++i) {
        seq_t vb = v_binom_prime(k, pf[i]);
        if (vb < (seq_t)(2 * ep[i]))
            return false;
    }

    /* check (k+2) */
    factorize(k + 2, pf, ep, &len);
    for (size_t i = 0; i < len; ++i) {
        seq_t vb = v_binom_prime(k, pf[i]);
        if (vb < (seq_t)(2 * ep[i]))
            return false;
    }

    return true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;

    init_primes();

    /* return cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* initialise cache storage if not yet done */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
    }

    /* find next terms until we reach the requested n */
    while (cache_len < n) {
        while (!satisfies(next_candidate))
            ++next_candidate;

        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            cache = tmp;
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
