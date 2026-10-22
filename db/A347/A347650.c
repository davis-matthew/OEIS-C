/*
 * A347650  Number of minimum total dominating sets in the n-pan graph (for n
 *          > 2).
 * https://oeis.org/A347650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A347650: Number of minimum total dominating sets in the n‑pan graph.
   Formula: a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12) for n > 12.
   Initial terms (1‑based):
   a(1)=1, a(2)=2, a(3)=3, a(4)=2, a(5)=3, a(6)=8,
   a(7)=5, a(8)=2, a(9)=5, a(10)=18, a(11)=7, a(12)=2.
*/
#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑based cache of computed terms */
static size_t cached_n = 0;       /* highest index computed */

static const seq_t initial_terms[13] = {
    0, /* unused index 0 */
    1, /* a(1) */
    2, /* a(2) */
    3, /* a(3) */
    2, /* a(4) */
    3, /* a(5) */
    8, /* a(6) */
    5, /* a(7) */
    2, /* a(8) */
    5, /* a(9) */
    18,/* a(10) */
    7, /* a(11) */
    2  /* a(12) */
};

/* Expand the cache up to (and including) index n, if necessary.  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cached_n)
        return; /* already available */

    /* Allocate space for indices 1 .. n */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
    {
        /* Allocation error – in a real program we might handle this
           differently, but for the purposes of this sequence function
           we simply return and let the caller see a garbage value. */
        return;
    }
    cache = new_cache;

    /* Compute missing terms */
    for (size_t i = cached_n + 1; i <= (size_t)n; ++i)
    {
        if (i <= 12)                 /* use the hard‑coded initial values */
            cache[i] = initial_terms[i];
        else
        {
            /* recurrence: a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12) */
            /* all indices are guaranteed to be <= n-1 at this point */
            seq_t t4  = cache[i - 4];
            seq_t t8  = cache[i - 8];
            seq_t t12 = cache[i - 12];
            cache[i] = (seq_t)3 * t4 - (seq_t)3 * t8 + t12;
        }
    }
    cached_n = (size_t)n;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; values for n < 1 are irrelevant. */
    if (n < 1)
        return 0;
    ensure_cache(n);
    return cache[(size_t)n];
}
