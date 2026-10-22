/*
 * A336658  Numbers k such that k and k+1 both have the prime signature
 *          (2,1,1,1) (A189982).
 * https://oeis.org/A336658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336658: numbers k such that both k and k+1 have prime signature (2,1,1,1).
   Implemented by incremental search with trial division using a precomputed prime list. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve (once, up to a fixed limit) ---------- */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_cnt    = 0;        /* number of primes stored */

/* generate primes up to LIMIT using the Sieve of Eratosthenes */
static void init_primes(void)
{
    if (prime_cnt != 0)      /* already initialized */
        return;

    /* LIMIT can be increased safely if needed. 1,000,000 covers
       all numbers we expect to test for the first ten thousand terms. */
    const uint64_t LIMIT = 1000000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;      /* allocation failed – should not happen */

    /* initialise */
    for (uint64_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* sieve */
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes and allocate array */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) { free(is_prime); return; }

    prime_cnt = cnt;
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = i;

    free(is_prime);
}

/* ---------- Cache for found sequence terms ---------- */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* ensure the cache has room for at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;          /* allocation failure – ignore in contest */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Test whether n has prime signature (2,1,1,1) ---------- */
static bool has_signature_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    init_primes();              /* make sure primes are available */

    uint64_t temp = n;
    int prime_factors = 0;
    bool square_found = false;

    /* trial division using the prime list */
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= temp / prime_list[i]; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 2)   /* exponent greater than 2 → invalid signature */
                    return false;
            }
            ++prime_factors;
            if (exp == 2) {
                if (square_found)
                    return false;   /* two squares would be needed */
                square_found = true;
            }
        }
    }
    if (temp > 1)            /* remaining prime factor with exponent 1 */
        ++prime_factors;

    return (prime_factors == 4 && square_found);
}

/* ---------- Main sequence accessor ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined only for n >= 1 */

    /* find next terms until we have at least n of them */
    static uint64_t next_candidate = 1;   /* start search from 1 */
    while (cache_len < (size_t)n) {
        uint64_t k = next_candidate;
        /* search for next eligible k */
        while (!(has_signature_uint64(k) && has_signature_uint64(k + 1))) {
            ++k;
        }
        /* store k */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)k;
        next_candidate = k + 1;   /* start next search after this term */
    }

    return cache[(size_t)n - 1];
}
