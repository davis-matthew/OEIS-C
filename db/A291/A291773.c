/*
 * A291773  Domination number of the n-Apollonian network.
 * https://oeis.org/A291773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n-3)+5)/2 for n > 2
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291773. Domination number of the n‑Apollonian network. */
/* Recurrence: a(n) = 4*a(n-1) - 3*a(n-2)  for n > 4. */
/* Initial values: a(1)=1, a(2)=1, a(3)=3, a(4)=4. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;     /* capacity of cache */
static size_t cache_len = 0;     /* number of elements stored */

/* Ensure cache has at least `n` elements, computing missing ones with the recurrence */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 4;
        cache_len = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)1;   /* a(2) */
        cache[2] = (seq_t)3;   /* a(3) */
        cache[3] = (seq_t)4;   /* a(4) */
    }

    if ((size_t)n <= cache_len) return;

    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 3*a(i-2)  for i > 4 */
        seq_t val = (cache[i-2] * (seq_t)4) - (cache[i-3] * (seq_t)3);
        cache[i-1] = val;
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but defined for safety */
    ensure_cache(n);
    return cache[n-1];
}
