/*
 * A364934  a(n+1) = 1 + number of previous terms that share a factor > 1 with
 *          a(n); a(1) = 2.
 * https://oeis.org/A364934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 339, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364934 – a(1) = 2; for n>1, a(n) = 1 + #{ k < n | gcd(a(k),a(n‑1)) > 1 } */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"              /* gcd(seq_t, seq_t) */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t *) */

static seq_t *cache = NULL;   /* cache[0] = a(1), ... */
static size_t cache_len = 0;  /* number of terms stored so far */
static size_t cache_cap = 0;  /* current capacity of the cache */

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    /* Allocate new space. */
    seq_t *new_arr = alloc_seq_array(new_cap);
    if (!new_arr)
        exit(EXIT_FAILURE); /* allocation failure */

    /* Copy existing terms. */
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];

    /* Free old array if it existed. */
    if (cache)
        free_seq_array(cache);

    cache = new_arr;
    cache_cap = new_cap;
}

/* Ensure that terms up to (and including) index n (1‑based) are computed. */
static void ensure_precomputed(idx_t n)
{
    if ((size_t)n <= cache_len)
        return;   /* already available */

    size_t target = (size_t)n;   /* number of terms needed */

    /* Grow capacity if needed. */
    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < target)
            new_cap *= 2;
        grow_cache(new_cap);
    }

    /* Compute new terms. */
    for (size_t i = cache_len; i < target; ++i) {
        if (i == 0) {                /* a(1) = 2 */
            cache[0] = (seq_t)2;
        } else {
            size_t cnt = 0;
            for (size_t j = 0; j < i; ++j) {
                if (gcd(cache[j], cache[i - 1]) > (seq_t)1)
                    ++cnt;
            }
            cache[i] = (seq_t)(1 + cnt);
        }
    }

    cache_len = target;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined below offset – behaviour ignored */
    ensure_precomputed(n);
    return cache[(size_t)n - 1];
}
