/*
 * A346628  G.f. A(x) satisfies: A(x) = 1 / (1 + x) + x * (1 + x) * A(x)^3.
 * https://oeis.org/A346628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(0) = 1, a(1) = 0, a(2) = 2. For
 *            n ≥ 3: a(n) = [(15n‑4)(n‑1)a(n‑1) + 2(n‑1)(21n‑22)a(n‑2) +
 *            23(n‑1)(n‑2)a(n‑3)] / [2n(2n+1)]
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

extern seq_t  a(idx_t n) ;   /* external linkage required */

/* Simple cache for previously computed values. */
static seq_t *cache     = NULL;  /* storage array */
static size_t cache_sz  = 0;     /* number of filled entries */
static size_t cache_cap = 0;     /* allocated slots */

/* Ensure the cache can hold at least 'need' entries. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t nw = (cache_cap > 0) ? cache_cap : 4;
    while (nw < need) nw <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, nw * sizeof(seq_t));
    if (!tmp) abort();   /* unrecoverable failure */
    cache = tmp;
    cache_cap = nw;
}

/* Compute a(n), reusing a simple recurrence. */
seq_t a(idx_t n)
{
    /* n may be larger than current cache size. */
    if (n >= cache_sz) {
        /* Allocate or enlarge cache. */
        ensure_cache((size_t)n + 1);

        /* Initialise base values if needed. */
        if (cache_sz == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)2;
            cache_sz = 3;
        }

        /* Fill cache up to n. */
        for (idx_t i = cache_sz; i <= n; ++i) {
            if (i < 3) continue;  /* base cases already set */

#if defined(__cplusplus)
#   include <type_traits>
    if constexpr (std::is_integral<seq_t>::value) {
        /* 64‑bit (or smaller) build – use 128‑bit intermediates. */
        __uint128_t t1 = (__uint128_t)(15 * i - 4) * (i - 1) *
                         ( __uint128_t)cache[i-1];
        __uint128_t t2 = (__uint128_t)2 * (i - 1) * (21 * i - 22) *
                         ( __uint128_t)cache[i-2];
        __uint128_t t3 = (__uint128_t)23 * (i - 1) * (i - 2) *
                         ( __uint128_t)cache[i-3];
        __uint128_t num = t1 + t2 + t3;
        __uint128_t den = (__uint128_t)2 * i * (2 * i + 1);
        cache[i] = (seq_t)(num / den);
    } else {
        /* Arbitrary‑precision build – native seq_t operations. */
        seq_t num = (seq_t)(15 * i - 4) * (i - 1) * cache[i-1];
        num += (seq_t)2 * (i - 1) * (21 * i - 22) * cache[i-2];
        num += (seq_t)23 * (i - 1) * (i - 2) * cache[i-3];
        seq_t den = (seq_t)2 * i * (2 * i + 1);
        cache[i] = num / den;
    }
#else
    /* Pure C (no __cplusplus); seq_t is always an unsigned integer type. */
    __uint128_t t1 = (__uint128_t)(15 * i - 4) * (i - 1) *
                     ( __uint128_t)cache[i-1];
    __uint128_t t2 = (__uint128_t)2 * (i - 1) * (21 * i - 22) *
                     ( __uint128_t)cache[i-2];
    __uint128_t t3 = (__uint128_t)23 * (i - 1) * (i - 2) *
                     ( __uint128_t)cache[i-3];
    __uint128_t num = t1 + t2 + t3;
    __uint128_t den = (__uint128_t)2 * i * (2 * i + 1);
    cache[i] = (seq_t)(num / den);
#endif
            ++cache_sz;
        }
    }
    return cache[n];
}
