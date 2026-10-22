/*
 * A365413  a(1) = 2, a(n) = k - 1, where k is the least number greater than
 *          a(n-1) such that rad(k) | a(n-1), where rad(n) = A007947(n).
 * https://oeis.org/A365413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = k - 1 where k is the least integer > a(n-1) such that
 *            rad(k) | a(n-1) (rad is the product of distinct prime factors)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/rad.h"

/* Cached values of the sequence, 0‑based: cache[0] = a(1) */
static seq_t *cache = NULL;
/* Number of allocated slots */
static size_t cache_cap = 0;
/* Number of terms already computed */
static size_t cache_used = 0;

/* Ensure that cache contains the first n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return; /* nothing to compute */

    /* Grow cache if necessary */
    if (cache_used < (size_t)n && cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    while (cache_used < (size_t)n) {
        if (cache_used == 0) {
            cache[0] = (seq_t)2;   /* a(1) = 2 */
        } else {
            seq_t prev = cache[cache_used - 1];
            idx_t k = (idx_t)prev + 1;

            /* Find the least k > prev with rad(k) | prev */
            for (;;) {
                seq_t radk = rad(k);   /* rad(k) as seq_t */
                if (prev % radk == 0)
                    break;
                ++k;
            }
            cache[cache_used] = (seq_t)k - 1;
        }
        ++cache_used;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
