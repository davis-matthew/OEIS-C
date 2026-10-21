/*
 * A253030  Number of (n+2) X (2+2) 0..2 arrays with every consecutive three
 *          elements in every row and column having exactly two distinct
 *          values, and new values 0 upwards introduced in row major order.
 * https://oeis.org/A253030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 5*a(n-2) - 10*a(n-3) - 174*a(n-4) + 80*a(n-5)
 *            + 160*a(n-6) Initial terms: a(1)=1647; a(2)=12987; a(3)=102060;
 *            a(4)=802899; a(5)=6320511; a(6)=49756086
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[1..len] holds a(1) .. a(len) */
static size_t cache_cap = 0;     /* allocated capacity (entries) */
static idx_t  cache_len = 0;     /* highest index computed */

/* Ensure at least 'needed' entries are allocated (indices 0..needed-1). */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1ull : 16ull;
    while (new_cap < needed)
        new_cap <<= 1ull;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);  /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function: returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n=0, but return 0 as safe default */

    /* If we already have it, return immediately */
    if (n <= cache_len && n >= 1)
        return cache[n];

    /* Ensure we have the initial terms stored */
    if (cache_len == 0) {
        ensure_capacity(7);
        cache[1] = (seq_t)1647;
        cache[2] = (seq_t)12987;
        cache[3] = (seq_t)102060;
        cache[4] = (seq_t)802899;
        cache[5] = (seq_t)6320511;
        cache[6] = (seq_t)49756086;
        cache_len = 6;
    }

    /* Generate terms up to n */
    ensure_capacity((size_t)n + 1);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 6) /* Already initialized */
            continue;
        seq_t t = 9ull * cache[i - 1];
        t -= 5ull * cache[i - 2];
        t -= 10ull * cache[i - 3];
        t -= 174ull * cache[i - 4];
        t += 80ull * cache[i - 5];
        t += 160ull * cache[i - 6];
        cache[i] = t;
    }
    cache_len = n;
    return cache[n];
}
