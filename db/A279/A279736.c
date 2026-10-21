/*
 * A279736  Number of n X 3 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 6, a(3) = 35, a(4) = 168, a(5) = 766, a(6) =
 *            3402, a(7) = 14827 For n ≥ 8: a(n) = 10·a(n-1) – 35·a(n-2) +
 *            54·a(n-3) – 45·a(n-4) + 20·a(n-5) – 4·a(n-6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* stored sequence values (1‑based indices) */
static size_t cache_size = 0;    /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure cache has at least 'needed' slots.  Assumes cache may be NULL. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;                /* double until big enough */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                      /* abort on out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but return 0 for safety */
        return (seq_t)0;

    if (!cache) {        /* initialise with first 7 terms */
        ensure_cache_capacity(7);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)35;
        cache[3] = (seq_t)168;
        cache[4] = (seq_t)766;
        cache[5] = (seq_t)3402;
        cache[6] = (seq_t)14827;
        cache_size = 7;
    }

    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    ensure_cache_capacity((size_t)n);

    for (size_t pos = cache_size; pos < (size_t)n; ++pos) {
        /* pos is the 0‑based index of the term to compute (a(pos+1)) */
        seq_t t1 = cache[pos - 1] * (seq_t)10   /* a(n-1)   */
                 + cache[pos - 3] * (seq_t)54   /* a(n-3)   */
                 + cache[pos - 5] * (seq_t)20;  /* a(n-5)   */
        seq_t t2 = cache[pos - 2] * (seq_t)35   /* a(n-2)   */
                 + cache[pos - 4] * (seq_t)45   /* a(n-4)   */
                 + cache[pos - 6] * (seq_t)4;   /* a(n-6)   */
        cache[pos] = t1 - t2;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
