/*
 * A394051  Hofstadter Q-sequence perturbed by (-1)^n.
 * https://oeis.org/A394051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(1)=a(2)=1; for n>2: a(n)=a(n-a(n-1))+a(n-a(n-2))+(-1)^n
 * Range:     u8 n <= 464, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
static seq_t *cache = NULL;          /* 1‑based array of computed terms */
static size_t cache_cap = 0;         /* allocated size (>= max index +1) */
static idx_t max_comp = 0;           /* largest n that has been computed */

/* Ensure cache[0..n] is allocated and zero‑initialized beyond the old size. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;               /* exponential growth */

    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1);                     /* out of memory */

    if (cache) {
        memcpy(new_ptr, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    /* new entries are zero (safe sentinel; all terms are odd) */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_ptr[i] = (seq_t)0;

    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined but harmless */

    ensure_cache(n);
    if (n <= max_comp)
        return cache[n];              /* already computed */

    /* compute terms from max_comp+1 up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = (seq_t)1;
        } else {
            /* compute indices for recursive calls */
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = i - (idx_t)cache[i - 2];
            seq_t s = cache[idx1] + cache[idx2];
            if ((i & 1ULL) == 0)        /* (-1)^i : +1 for even i */
                s += (seq_t)1;
            else                       /* -1 for odd i */
                s -= (seq_t)1;
            cache[i] = s;
        }
    }
    max_comp = n;
    return cache[n];
}
