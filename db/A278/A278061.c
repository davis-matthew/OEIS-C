/*
 * A278061  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 9; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 9.
 * https://oeis.org/A278061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 9; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n
 *            > 9
 * Range:     u8 n <= 386, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* 1‑indexed array of computed values   */
static size_t current_max = 0;       /* highest index that has been computed */

/* Allocate or grow cache so that indices up to n are valid.
   Compute all missing values in increasing order. */
static void ensure_cache(idx_t n)
{
    size_t old_max = current_max;
    size_t need    = (size_t)n;

    /* Grow the cache if necessary */
    if (need > old_max) {
        size_t new_cap = need + 1;   /* +1 for 1‑based indexing */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failure */
        cache = tmp;
        /* Zero the new part to avoid garbage */
        memset(&cache[old_max + 1], 0, (new_cap - old_max - 1) * sizeof(seq_t));

        /* Initialise base values */
        if (old_max < 9) {
            size_t start = old_max + 1;
            if (start < 1) start = 1;
            for (idx_t i = start; i <= 9 && i <= n; ++i)
                cache[(size_t)i] = (seq_t)i;
        }

        current_max = need;
    }

    /* Compute any missing values that depend only on smaller indices */
    for (size_t i = old_max + 1; i <= need; ++i) {
        if ((idx_t)i <= 9) continue;  /* already initialized */
        /* indices are guaranteed to be < i, so cache entries exist */
        idx_t j1 = (idx_t)i - (idx_t)cache[(size_t)(i - 1)];
        idx_t j2 = (idx_t)i - (idx_t)cache[(size_t)(i - 2)];
        cache[(size_t)i] = cache[(size_t)j1] + cache[(size_t)j2];
    }
}

/* Public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but safe */
    ensure_cache(n);
    return cache[(size_t)n];
}
