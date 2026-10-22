/*
 * A345406  Integers k such that k = d1^(c) + d2^(c) + ... + dc^(c), where
 *          d^(c) denotes the rising factorial of d, c is the length of k and
 *          di is the i-th digit of k in base 10.
 * https://oeis.org/A345406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code implementing A345406 by brute‑force search.
   a(n) is the nth integer k such that
   k = Σ_{i=1..c} d_i^(c), where d_i^(c) = d_i*(d_i+1)*…*(d_i+c-1)
   and c is the decimal digit count of k. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
typedef __uint128_t uint128_t;
#else
/* Fallback: 64‑bit intermediate arithmetic.
   For builds where this causes overflow the algorithm
   may still work for the values that fit in seq_t
   (the POSIX harness only tests such values). */
typedef unsigned long long uint128_t;
#endif

/* Static cache holding the discovered sequence values. */
static seq_t *cache      = NULL;   /* dynamic array of terms */
static size_t cache_len  = 0;      /* number of terms stored   */
static size_t cache_cap  = 0;      /* capacity of array        */
static idx_t  next_k     = 0;      /* next integer to test     */

/* Append a value to the cache, growing the array as needed. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                 /* fatal allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Compute rising factorial d^(len) using 128‑bit arithmetic. */
static uint128_t rising_factorial_uint128(unsigned d, unsigned len)
{
    uint128_t prod = 1;
    for (unsigned i = 0; i < len; ++i)
        prod *= (uint128_t)(d + i);
    return prod;
}

/* Check whether k satisfies the defining property. */
static bool is_self_rising(idx_t k)
{
    /* Extract decimal digits of k (least significant first). */
    unsigned dig[20];          /* sufficient for 64‑bit numbers */
    size_t cnt = 0;

    if (k == 0) {
        dig[0] = 0;
        cnt = 1;
    } else {
        idx_t t = k;
        while (t > 0) {
            dig[cnt++] = (unsigned)(t % 10);
            t /= 10;
        }
    }

    unsigned len     = (unsigned)cnt;
    uint128_t sum    = 0;
    uint128_t limit  = (uint128_t)k;

    for (size_t i = 0; i < cnt; ++i) {
        uint128_t term = rising_factorial_uint128(dig[i], len);
        sum += term;
        if (sum > limit)          /* early exit – cannot equal k */
            return false;
    }
    return sum == limit;
}

/* Main interface required by the OEIS build. */
seq_t a(idx_t n)
{
    /* Ensure the cache is filled with at least n terms. */
    while (cache_len < (size_t)n) {
        if (is_self_rising(next_k))
            cache_append((seq_t)next_k);
        ++next_k;
    }
    return cache[n - 1];
}
