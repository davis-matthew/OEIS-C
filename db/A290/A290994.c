/*
 * A290994  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S^7.
 * https://oeis.org/A290994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + 2*a(n-7) for n >= 8 a(0)=0, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=0, a(5)=0, a(6)=1, a(7)=7
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Multiplication by a small unsigned integer.
   For C++ builds the template takes care of arbitrary‑precision types.
   For C builds the intermediate uses 128‑bit arithmetic to avoid overflow. */
#ifndef __cplusplus
static inline seq_t mul_uint(seq_t x, unsigned int c)
{
    __uint128_t tmp = (__uint128_t)x * c;
    return (seq_t)tmp;
}
#else
template<typename T>
static inline T mul_uint(T x, unsigned int c) { return x * c; }
#endif

/* Static cache for computed values.  Initially empty. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Fill the cache up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* nothing to do */

    /* Allocate or enlarge the array. */
    idx_t new_len = (n + 1 > cache_len) ? (n + 1) : cache_len;
    if (new_len > cache_len) {
        seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!p) abort();                /* allocation failure */
        cache = p;
    }

    /* Initialise the first eight terms if necessary. */
    if (cache_len < 8) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache[5] = 0;
        cache[6] = 1;
        cache[7] = 7;
        cache_len = 8;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* i >= 8 because cache_len >=8 and i starts from it. */
        seq_t pos = mul_uint(cache[i-1], 7);
        pos += mul_uint(cache[i-3], 35);
        pos += mul_uint(cache[i-5], 21);
        pos += mul_uint(cache[i-7], 2);

        seq_t neg = mul_uint(cache[i-2], 21);
        neg += mul_uint(cache[i-4], 35);
        neg += mul_uint(cache[i-6], 7);

        cache[i] = pos - neg;
    }
    cache_len = new_len;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 8) {                      /* term fully known */
        if (cache_len < 8) {          /* initialise cache once */
            ensure_cache(7);
        }
        return cache[n];
    }

    ensure_cache(n);
    return cache[n];
}
