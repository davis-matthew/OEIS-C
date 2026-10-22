/*
 * A327541  A linear divisibility sequence of order 8.
 * https://oeis.org/A327541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) - 16*a(n-4) - 4*a(n-5) +
 *            a(n-6) + 4*a(n-7) - a(n-8). Initial values:
 *            a(1)=1,a(2)=4,a(3)=16,a(4)=72,a(5)=271,
 *            a(6)=1024,a(7)=3809,a(8)=13968
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static array of the first 8 terms. */
static const seq_t initVals[8] = {
    (seq_t)1,   (seq_t)4,   (seq_t)16,  (seq_t)72,
    (seq_t)271, (seq_t)1024,(seq_t)3809,(seq_t)13968
};

/* Cached sequence values and current cached size. */
static seq_t *cache = NULL;
static size_t  cap   = 0;

/* Grow the cache to hold up to index `n` (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                     /* undefined, ignore */
    size_t old_cap = cap;
    if ((size_t)n <= cap) return;           /* already sufficient */

    /* Allocate new larger buffer. */
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;                  /* allocation failed – unlikely */

    /* Copy previously stored terms. */
    if (cache) {
        for (size_t i = 0; i < old_cap; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cap   = new_cap;

    /* Compute or copy terms from old_cap+1 up to new_cap. */
    for (size_t i = old_cap + 1; i <= new_cap; ++i) {
        if (i <= 8) {
            /* First eight terms are known constants. */
            cache[i - 1] = initVals[i - 1];
        } else {
            /* Use the linear recurrence of order 8. */
            seq_t val = 0;
            val = val + (seq_t)4 * cache[i - 2];                 /* a(i-1) */
            val = val + cache[i - 3];                            /* a(i-2) */
            val = val - (seq_t)4 * cache[i - 4];                 /* a(i-3) */
            val = val - (seq_t)16 * cache[i - 5];                /* a(i-4) */
            val = val - (seq_t)4 * cache[i - 6];                 /* a(i-5) */
            val = val + cache[i - 7];                            /* a(i-6) */
            val = val + (seq_t)4 * cache[i - 8];                 /* a(i-7) */
            val = val - cache[i - 9];                            /* a(i-8) */
            cache[i - 1] = val;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0, return 0 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
