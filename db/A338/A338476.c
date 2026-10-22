/*
 * A338476  Primes of the form 398*x^2-1.
 * https://oeis.org/A338476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form 398*x^2-1,
 *            a(n)=398*A338477(n)^2-1
 * Range:     u8 none, u16 n <= 9, u32 n <= 10, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached prime values */
static size_t cache_len = 0;         /* number of cached primes */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t max_x = 0;              /* largest x already tested */

/* Expand the cache to hold at least |len| elements. */
static void ensure_cache_capacity(size_t len)
{
    if (len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < len) new_cap <<= 1;

#ifdef __cplusplus
    /* C++: allocate with new[], copy old contents, delete old array. */
    seq_t *new_ptr = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_ptr[i] = cache[i];
        delete[] cache;
    }
    cache = new_ptr;
    cache_cap = new_cap;
#else
    /* C: use realloc. */
    seq_t *new_ptr = realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE);
    cache = new_ptr;
    cache_cap = new_cap;
#endif
}

/* Return the n‑th prime of the form 398*x^2-1 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1 */

    /* Keep generating candidates until we have enough prime terms. */
    while (cache_len < (size_t)n) {
        ++max_x;
        unsigned long long xx = (unsigned long long)max_x;

        /* Compute 398*xx*xx - 1 using 128‑bit arithmetic to avoid overflow. */
        __uint128_t cand128 = (__uint128_t)398 * xx * xx - 1;
        seq_t val = (seq_t)cand128;

        if (is_prime_big(val)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
    }
    return cache[n - 1];
}
