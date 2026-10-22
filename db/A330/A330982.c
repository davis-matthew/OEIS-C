/*
 * A330982  Remeven numbers: having an even remainder modulo any of their
 *          digits, digit 0 forbidden.
 * https://oeis.org/A330982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   remeven_ok, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition:
   a(n) is the n‑th positive integer x such that
   • every decimal digit of x is in {1,…,9} (no zeros)
   • for each digit d of x, the remainder of x modulo d is even.
   a(1) = 1.  */

/* Note:  seq_t may be an ordinary integer type or an arbitrary‑precision integer class.
   No rotations or complex dependencies.  The function caches results in a static
   dynamically resizable array allocated with the helper
   seq_realloc (see ../utils/seq_realloc.h).  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"
#include "../utils/remeven_ok.h"

/* ------------------------------------------------------------------ */
/*  Helper: test whether a number satisfies the Remeven property      */

/* ------------------------------------------------------------------ */
/*  Cache of previously computed terms                                 */
/* ------------------------------------------------------------------ */
static seq_t *cache   = NULL;   /* pointer to array of seq_t          */
static size_t cache_used = 0;   /* how many terms are stored          */
static size_t cache_cap  = 0;   /* current allocated capacity         */

/* ------------------------------------------------------------------ */
/*  Grow the cache to accommodate at least min_cap elements           */
/* ------------------------------------------------------------------ */
static void grow_cache(size_t min_cap)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;

    /* seq_realloc returns the reallocated pointer                               */
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp && new_cap > 0) {
        /* Out of memory – terminate the program (the tests never hit this).    */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                   /* offset is 1 – undefined for n=0 */
        return (seq_t)0;

    /* Generate terms until the cache contains at least n elements. */
    if (cache_used < (size_t)n) {
        /* Ensure capacity. */
        if (cache_cap < (size_t)n)
            grow_cache((size_t)n);

        /* Start checking from the next integer after the last cached one. */
        seq_t cur = (cache_used == 0) ? (seq_t)1
                                      : cache[cache_used - 1] + 1;

        while (cache_used < (size_t)n) {
            if (remeven_ok(cur)) {
                cache[cache_used] = cur;
                ++cache_used;
            }
            cur += 1;           /* next candidate integer   */
        }
    }

    return cache[(size_t)n - 1];
}
