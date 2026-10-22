/*
 * A329952  Numbers k such that binomial(k,3) is divisible by 8.
 * https://oeis.org/A329952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 6 Initial values:
 *            a(1)=0, a(2)=1, a(3)=2, a(4)=8, a(5)=10, a(6)=16. The sequence
 *            consists of all integers k with 8 | binomial(k,3)
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return the n-th term of sequence A329952 (n >= 1). */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms, 1‑based indexing */
    static seq_t *cache      = NULL; /* array of terms */
    static size_t  cache_cap = 0;    /* allocated capacity (including index 0) */
    static size_t  cache_len = 0;    /* highest n for which cache[n] is valid */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with room for first 7 entries (index 0 unused) */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            abort();               /* allocation failed */

        /* initialise first six terms */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)10;
        cache[6] = (seq_t)16;
        cache_len = 6;
    }

    /* expand cache if needed */
    if ((size_t)n > cache_len) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap = new_cap * 2;   /* geometric expansion for speed */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();               /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if they are not present */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 6) {               /* should not happen after init, but keep safe */
            switch ((int)i) {
            case 1: cache[i] = (seq_t)0; break;
            case 2: cache[i] = (seq_t)1; break;
            case 3: cache[i] = (seq_t)2; break;
            case 4: cache[i] = (seq_t)8; break;
            case 5: cache[i] = (seq_t)10; break;
            case 6: cache[i] = (seq_t)16; break;
            }
        } else {
            /* recurrence: a(n) = a(n-1) + a(n-5) - a(n-6) */
            cache[i] = cache[i - 1] + cache[i - 5] - cache[i - 6];
        }
    }
    cache_len = (size_t)n;

    return cache[n];
}
