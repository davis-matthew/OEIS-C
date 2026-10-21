/*
 * A278056  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 4; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 4.
 * https://oeis.org/A278056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=4; For n>4:
 *            a(n)=a(n-a(n-1))+a(n-a(n-2))
 * Range:     u8 n <= 430, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */
#include <stddef.h>   /* for size_t */

/* External types supplied by the build environment */
extern seq_t a(idx_t n);  /* already declared elsewhere; we re‑define it here */

/* Implementation of the recurrence with memoisation */
seq_t a(idx_t n)
{
    /* 1‑based cache: cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t len  = 0;  /* next free index */
    static size_t cap  = 0;  /* allocated capacity */

    /* allocate the cache if it does not exist yet */
    if (!cache) {
        cap = 5;                     /* indices 0..4 */
        cache = (seq_t*)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Ensure capacity for indices up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise first four values if necessary */
    if (len <= 4) {
        for (idx_t i = 1; i <= 4; ++i)
            cache[(size_t)i] = (seq_t)i;
        len = 5;  /* next free index */
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* indices required by the recurrence */
        idx_t idx1 = i - (idx_t)cache[(size_t)(i - 1)];
        idx_t idx2 = i - (idx_t)cache[(size_t)(i - 2)];
        cache[(size_t)i] = cache[(size_t)idx1] + cache[(size_t)idx2];
    }
    len = (size_t)(n + 1);  /* we now have indices up to n */

    return cache[(size_t)n];
}
