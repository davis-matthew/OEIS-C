/*
 * A288841  Number of matchings in the n-barbell graph.
 * https://oeis.org/A288841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000085(n)^2 + A000085(n-1)^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *inv_cache = NULL;   /* cache of A000085 values                */
static idx_t cache_size = 0;      /* number of initialized entries (>=2)    */
static idx_t cache_cap   = 0;      /* allocated capacity in entries          */

/* Ensure the cache holds values up to 'need' (0‑based index).   */
static void ensure_cache(idx_t need)
{
    idx_t required = need + 1;        /* indices 0 … need inclusive          */
    if (required <= cache_cap)
        return;

    /* Grow the capacity, at least doubling at a time.               */
    idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap < required)
        new_cap <<= 1;

    inv_cache = seq_realloc(inv_cache, cache_cap, new_cap);
    cache_cap = new_cap;

    /* Initialise base cases if this is the first allocation.       */
    if (cache_size == 0) {
        inv_cache[0] = (seq_t)1;
        inv_cache[1] = (seq_t)1;
        cache_size   = 2;
    }
}

/* Return a(n) for any n >= 1.  The definition uses A000085.        */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, safe fallback           */

    ensure_cache(n);

    /* Compute any missing values up to n using the recurrence.     */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t term = inv_cache[k-1] + (seq_t)(k-1) * inv_cache[k-2];
        inv_cache[k] = term;
    }
    cache_size = n + 1;

    seq_t fn  = inv_cache[n];
    seq_t fn1 = inv_cache[n-1];
    return fn * fn + fn1 * fn1;
}
