/*
 * A392698  Numbers whose reversed prime indices are the 0-prepended first
 *          sums of some finite sequence of distinct integers.
 * https://oeis.org/A392698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A392698: numbers whose reversed prime indices are the
   0‑prepended first sums of some finite sequence of distinct integers.
   Implementation relies on trial‑division factorisation and a
   dynamically grown list of primes. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Prime generation --------------------------------------------------- */
static uint64_t *prime_list = NULL;     /* array of primes (1‑based indices) */
static size_t   prime_cnt   = 0;         /* number of primes stored */
static uint64_t sieve_limit = 0;         /* largest prime considered */

/* Sieve of Eratosthenes up to 'limit' (inclusive) */
static void ensure_prime_limit(uint64_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate boolean array for sieve */
    char *is_prime = (char *)malloc(limit + 1);
    if (!is_prime) exit(1);

    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes */
    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++new_cnt;

    /* Resize prime list */
    uint64_t *new_primes = (uint64_t *)realloc(prime_list, new_cnt * sizeof(uint64_t));
    if (!new_primes) exit(1);
    prime_list = new_primes;
    prime_cnt  = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = i;

    free(is_prime);
    sieve_limit = limit;
}

/* Binary search for 1‑based index of prime 'p' */
static inline uint64_t prime_index(uint64_t p)
{
    ensure_prime_limit(p);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else if (prime_list[mid] > p) hi = mid;
        else return mid + 1;  /* 1‑based */
    }
    return 0;  /* should not happen for prime p */
}

/* --- Cache for computed sequence ---------------------------------------- */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;

/* Grow cache capacity as needed */
static void cache_push(seq_t val)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* --- Property check ----------------------------------------------------- */
static bool satisfies_uint64(uint64_t k)
{
    if (k == 1) return true;  /* 1 has empty prime index list */

    /* Factor k and collect prime indices (1‑based) with multiplicity */
    uint64_t prime_indices[64];
    size_t len = 0;
    uint64_t n = k;

    /* Ensure prime list covers possible prime factors */
    uint64_t limit = 2;
    for (uint64_t q = 2; q * q <= n; ++q) limit = q + 1;
    if (limit > 1) ensure_prime_limit(limit);

    size_t pi = 0;
    while (n > 1 && pi < prime_cnt && prime_list[pi] * prime_list[pi] <= n) {
        uint64_t p = prime_list[pi++];
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            uint64_t idx = prime_index(p);
            for (uint64_t e = 0; e < exp; ++e) {
                if (len < sizeof(prime_indices)/sizeof(prime_indices[0])) {
                    prime_indices[len++] = idx;
                }
            }
        }
    }
    if (n > 1) {
        uint64_t idx = prime_index(n);
        if (len < sizeof(prime_indices)/sizeof(prime_indices[0]))
            prime_indices[len++] = idx;
    }

    /* Reverse the list to obtain q */
    for (size_t i = 0; i < len/2; ++i) {
        uint64_t t = prime_indices[i];
        prime_indices[i] = prime_indices[len-1-i];
        prime_indices[len-1-i] = t;
    }

    /* Reconstruct sequence a: a[0] = q[0]; a[i] = q[i] - a[i-1] */
    int64_t a[64];
    if (len == 0) return true;  /* shouldn't happen except for k=1 */
    a[0] = (int64_t)prime_indices[0];
    for (size_t i = 1; i < len; ++i) {
        a[i] = (int64_t)prime_indices[i] - a[i-1];
    }

    /* Check distinctness of a[0..len-1] */
    for (size_t i = 0; i < len; ++i)
        for (size_t j = i + 1; j < len; ++j)
            if (a[i] == a[j]) return false;

    return true;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n terms cached */
    uint64_t candidate = 1;
    if (cache_sz > 0)
        candidate = (uint64_t)cache[cache_sz - 1] + 1;

    while (cache_sz < (size_t)n) {
        if (satisfies_uint64(candidate))
            cache_push((seq_t)candidate);
        ++candidate;
    }

    return cache[n - 1];
}
