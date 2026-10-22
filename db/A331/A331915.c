/*
 * A331915  Numbers with exactly one prime prime index, counted with
 *          multiplicity.
 * https://oeis.org/A331915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A331915 – numbers with exactly one prime prime‑index factor,
 *            counted with multiplicity
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* ------------------------------------------------------------------ */
/* Global data and initialisation                                      */
/* ------------------------------------------------------------------ */
static bool initialized = false;

static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static unsigned char *prime_index_is_prime = NULL; /* 1..primes_cnt inclusive */

#define LIMIT 10000000  /* initial sieve limit */

static void init_sequence(void)
{
    size_t m = LIMIT;
    unsigned char *is_prime = (unsigned char *)malloc(m + 1);
    assert(is_prime);
    for (size_t i = 0; i <= m; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= m; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= m; j += i)
                is_prime[j] = 0;
        }
    }
    /* count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= m; ++i)
        if (is_prime[i]) ++primes_cnt;
    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    assert(primes);
    size_t pos = 0;
    for (size_t i = 2; i <= m; ++i)
        if (is_prime[i]) primes[pos++] = i;
    /* build prime‑index primality array */
    prime_index_is_prime = (unsigned char *)malloc((primes_cnt + 1) * sizeof(unsigned char));
    assert(prime_index_is_prime);
    for (size_t idx = 1; idx <= primes_cnt; ++idx)
        prime_index_is_prime[idx] = is_prime[idx];
    free(is_prime);
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Helper: binary search for prime index (1‑based)                     */
/* ------------------------------------------------------------------ */
static size_t prime_index_uint64(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t v = primes[mid];
        if (v == p) return mid + 1;          /* 1‑based index */
        if (v < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* not found */
}

/* ------------------------------------------------------------------ */
/* Test whether a 64‑bit integer satisfies the condition             */
/* ------------------------------------------------------------------ */
static bool qualifies_uint64(uint64_t n)
{
    uint64_t temp = n;
    int multiplicity = 0;
    bool has = false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > temp / p) break;
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            size_t idx = prime_index_uint64(p);
            if (idx && prime_index_is_prime[idx]) {
                has = true;
                multiplicity += cnt;
                if (multiplicity > 1) break;  /* early exit if too many */
            }
        }
    }
    if (multiplicity > 1) return false;
    if (temp > 1) {                   /* remaining prime factor */
        size_t idx = prime_index_uint64(temp);
        if (idx && prime_index_is_prime[idx]) {
            has = true;
            multiplicity += 1;
            if (multiplicity > 1) return false;
        }
    }
    return has && multiplicity == 1;
}

/* ------------------------------------------------------------------ */
/* Cache of sequence values                                            */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_candidate = 3;

static void ensure_cache(idx_t n)
{
    if (!initialized) init_sequence();
    while (cache_len < (size_t)n) {
        while (!qualifies_uint64(next_candidate))
            ++next_candidate;
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            assert(cache);
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Required public function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined, safety */
    ensure_cache(n);
    return cache[n - 1];
}
