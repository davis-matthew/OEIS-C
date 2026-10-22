/*
 * A302506  Number of total dominating sets in the n-pan graph.
 * https://oeis.org/A302506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=3, a(3)=7, a(4)=12, for n>4:
 *            a(n)=a(n-1)+a(n-3)+a(n-4) (OEIS A302506)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based storage, index 0 unused */
static size_t cache_cap = 0;     /* current allocated size (number of slots) */
static idx_t last_computed = 0;  /* largest n already stored */

/* enlarge cache so that indices up to n are valid */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;                  /* need indices 0..n */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* out of range */

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 8;                   /* indices 0..7, more than enough for initial values */
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)12;
        /* remaining indices currently zero */
        last_computed = 4;
    }

    if (n <= last_computed)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(n)=a(n-1)+a(n-3)+a(n-4) for n>4 */
        cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }
    last_computed = n;
    return cache[n];
}
