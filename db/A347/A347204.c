/*
 * A347204  a(n) = a(f(n)/2) + a(floor((n+f(n))/2)) for n > 0 with a(0) = 1
 *          where f(n) = A129760(n).
 * https://oeis.org/A347204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = a(n - 2^f(n)) + (1 + f(n))*a((n - 2^f(n))/2), a(0) = 1,
 *            where f(n) = number of trailing zeros in the binary expansion of
 *            n
 * Range:     u8 n <= 54, u16 n <= 508, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/seq_free_array.h"   /* void seq_free_array(seq_t* p); */

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of entries currently in cache */

/* trailing‑zero exponent of n (n>0) */
static inline unsigned v2(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned)__builtin_ctzll((unsigned long long)n);
#else
    unsigned d = 0;
    while ((n & 1ULL) == 0) { n >>= 1; ++d; }
    return d;
#endif
}

/* Ensure that cache contains values for all indices 0 … n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;                /* already computed */

    idx_t new_size = n + 1;
    seq_t *new_cache = alloc_seq_array((size_t)new_size);
    if (!new_cache) exit(EXIT_FAILURE);        /* out of memory */

    /* copy old values */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        seq_free_array(cache);
    }

    /* initialise new entries */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {                            /* a(0) = 1 */
            new_cache[0] = (seq_t)1;
            continue;
        }
        unsigned f = v2(i);
        idx_t two = ((idx_t)1) << f;              /* 2^f as idx_t */
        idx_t sub = i - two;                      /* i - 2^f */
        seq_t term1 = new_cache[sub];
        seq_t term2 = new_cache[sub >> 1];
        new_cache[i] = term1 + ((seq_t)(1 + f)) * term2;
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
