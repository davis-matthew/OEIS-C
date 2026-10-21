/*
 * A274557  Numbers k such that sigma(k) == 0 (mod k+6).
 * https://oeis.org/A274557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274557: k such that sigma(k) ≡ 0 (mod k+6) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Prime sieve for numbers up to a fixed limit */
static uint32_t *primes = NULL;
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Build a list of primes up to LIMIT */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* Limit chosen to cover sqrt of all k we test (<< 1 000 000). */
    enum { LIMIT = 2000000 };
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    primes = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    prime_count = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_count++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_initialized = true;
}

/* Sigma of n: sum of all positive divisors of n */
static seq_t compute_sigma(idx_t n)
{
    seq_t sigma = 1;
    idx_t temp = n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp)
            break;
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t p_pow = 1;
            for (idx_t e = 0; e <= exp; ++e)
                p_pow *= (seq_t)p;          /* p^(exp+1) */
            seq_t term = (p_pow - 1) / ((seq_t)p - 1); /* (p^(exp+1)-1)/(p-1) */
            sigma *= term;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        seq_t term = (seq_t)temp + 1;
        sigma *= term;
    }
    return sigma;
}

/* The required function, memoised and order‑independent */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined for this offset */

    init_primes();

    /* Static storage so repeated calls are efficient */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_scanned = 0;          /* greatest k processed so far */

    /* Return immediately if we already have the term */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if needed */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Search for new terms until we have at least n of them */
    idx_t k = last_scanned + 1;
    while (cache_len < n) {
        seq_t sigma_k = compute_sigma(k);
        if (sigma_k % (seq_t)(k + 6) == 0) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++k;
    }
    last_scanned = k - 1;
    return cache[n - 1];
}
