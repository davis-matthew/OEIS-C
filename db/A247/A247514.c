/*
 * A247514  Numbers k such that 2*floor(sqrt(prime(k))) =
 *          floor(2*sqrt(prime(k))).
 * https://oeis.org/A247514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where 2*floor(sqrt(p_k)) = floor(2*sqrt(p_k)) for p_k
 *            the k-th prime. a(1)=1, a(2)=3, a(3)=5, …
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Utilities */
#include "../utils/build_primes_upto.h"

/* ---------- helper for integer sqrt of uint64_t ---------- */
static uint64_t _isqrt_u64(uint64_t n) {
    uint64_t x = 0, y = 1ULL << 32;
    while (y) {
        uint64_t d = x + y;
        if (d <= n / d) { /* avoid overflow of d*d */
            x = d;
        }
        y >>= 1;
    }
    return x;
}

/* ---------- static state ---------- */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static size_t   primes_limit = 0;

static seq_t   *cache = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;

/* ---------- memory helpers for cache (C / C++) ---------- */
#ifdef __cplusplus
static void free_cache_array(seq_t *p) { delete[] p; }
static seq_t* alloc_cache_array(size_t cap) { return new seq_t[cap]; }
#endif

static void free_cache(seq_t *p) {
#ifdef __cplusplus
    free_cache_array(p);
#else
    free(p);
#endif
}

static seq_t* alloc_cache(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)calloc(cap, sizeof(seq_t));
#endif
}

static void enlarge_cache(size_t new_cap) {
    seq_t *newp = alloc_cache(new_cap);
    for (size_t i = 0; i < cache_len; ++i) newp[i] = cache[i];
    free_cache(cache);
    cache = newp;
    cache_cap = new_cap;
}

/* ---------- rebuild cache up to a given value limit ---------- */
static void rebuild_cache(size_t limit) {
    /* free old primes */
    if (primes) free(primes);

    /* build new primes */
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;

    /* free old cache and allocate fresh small cache */
    free_cache(cache);
    cache_cap = 4;
    cache_len = 0;
    cache = alloc_cache(cache_cap);

    /* test each prime */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t a = _isqrt_u64(p);
        uint64_t bound = a * a + a;
        if (p <= bound) {
            if (cache_len == cache_cap) enlarge_cache(cache_cap * 2);
            cache[cache_len++] = (seq_t)(i + 1);  /* 1‑based index */
        }
    }
}

/* ---------- ensure at least 'req' terms in cache ---------- */
static void ensure_cache(size_t req) {
    if (cache_len >= req) return;
    size_t limit = primes_limit ? primes_limit * 2 : 100000;
    while (cache_len < req) {
        rebuild_cache(limit);
        if (cache_len >= req) break;
        limit <<= 1;
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (cache_len < (size_t)n) ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
