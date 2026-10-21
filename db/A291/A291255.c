/*
 * A291255  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S - 2 S^2)^2.
 * https://oeis.org/A291255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: recurrence for n ≥ 9
 *   a(n) = 2*a(n-1) + 7*a(n-2) - 10*a(n-3) - 16*a(n-4) + 10*a(n-5)
 *          + 7*a(n-6) - 2*a(n-7) - a(n-8)
 * Initial terms (n = 0 … 8):
 *   a(0)=2, a(1)=7, a(2)=18, a(3)=55, a(4)=144,
 *   a(5)=404, a(6)=1060, a(7)=2853, a(8)=7442
 * This function memoises computed values in a dynamic array; it works
 * for any uint8_t, uint16_t, uint32_t, uint64_t or arbitrary‑precision
 * integer type supplied for seq_t.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc, free, abort */

/* Forward declaration required by the harness */
seq_t a(idx_t n);

/* --- memoisation cache ---------------------------------------------- */

static seq_t *cache = NULL;          /* array of computed terms   */
static idx_t  cache_size = 0;        /* number of elements stored */
static idx_t  cache_cap   = 0;        /* allocated capacity        */

/* first nine terms (used on initialisation) */
static const seq_t init_terms[9] = {
    (seq_t)2,   /* a(0) */
    (seq_t)7,   /* a(1) */
    (seq_t)18,  /* a(2) */
    (seq_t)55,  /* a(3) */
    (seq_t)144, /* a(4) */
    (seq_t)404, /* a(5) */
    (seq_t)1060,/* a(6) */
    (seq_t)2853,/* a(7) */
    (seq_t)7442 /* a(8) */
};

/* ensure stack has room for indices 0 … n (inclusive).
   The routine grows the array on demand and populates missing
   values using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;       /* already have needed terms */

    /* grow the allocation to at least needed size */
    size_t needed = (size_t)n + 1;
    while (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? 9 : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* initialise the first nine terms on first use */
    if (cache_size == 0) {
        for (idx_t i = 0; i < 9; ++i) cache[i] = init_terms[i];
        cache_size = 9;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] =
              (seq_t)2 * cache[i-1]
            + (seq_t)7 * cache[i-2]
            - (seq_t)10 * cache[i-3]
            - (seq_t)16 * cache[i-4]
            + (seq_t)10 * cache[i-5]
            + (seq_t)7  * cache[i-6]
            - (seq_t)2 * cache[i-7]
            - cache[i-8];
    }
    cache_size = n + 1;
}

/* ------------------------------------------------------------ */
/* Public entry point: returns a(n) for any n ≥ 0. */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 9) return init_terms[n];
    ensure_cache(n);
    return cache[(size_t)n];
}
