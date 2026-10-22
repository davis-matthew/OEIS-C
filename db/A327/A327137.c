/*
 * A327137  Numbers k such that sin(2k) > sin(2k+2) > sin(2k+4) < sin(2k+6).
 * https://oeis.org/A327137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS A327137 (Numbers k such that
   sin(2k) > sin(2k+2) > sin(2k+4) < sin(2k+6)).
   The function does a lazy scan of successive integers k,
   testing the inequality with long‑double sine values and
   storing the qualifying k's in a static cache. */
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/* Lazy cache for the sequence values */
static seq_t *cache      = NULL; /* array of sequence terms */
static size_t cache_len  = 0;    /* number of terms cached so far */
static size_t cache_cap  = 0;    /* current allocated capacity of cache */
static idx_t next_k      = 0;    /* next integer k to test */

/* Test whether a given k satisfies the inequality */
static inline bool k_satisfies(idx_t k)
{
    long double a = sinl(2.0L * (long double)k);
    long double b = sinl(2.0L * (long double)(k + 1));
    long double c = sinl(2.0L * (long double)(k + 2));
    long double d = sinl(2.0L * (long double)(k + 3));
    return (a > b) && (b > c) && (c < d);
}

/* Grow the cache array to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* Start with a small capacity if none allocated yet */
    if (cache_cap == 0) grow_cache(16);

    while (cache_len < (size_t)n) {
        if (k_satisfies(next_k)) {
            if (cache_len == cache_cap) grow_cache(cache_cap * 2);
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }
}

/* Sequence function required by the framework */
seq_t a(idx_t n)
{
    /* The sequence is indexed from 1 */
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n - 1];
}
