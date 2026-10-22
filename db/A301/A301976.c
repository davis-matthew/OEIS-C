/*
 * A301976  Number of no-leaf subgraphs of the 3 X n grid.
 * https://oeis.org/A301976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 12*a(n-1) - 6*a(n-2) - 20*a(n-3) - 5*a(n-4)   (n > 4)
 * Initial values:
 *   a(1) = 1
 *   a(2) = 5
 *   a(3) = 43
 *   a(4) = 463
 *
 * The function memoises computed values in a static dynamic array so
 * that it can answer any call order in O(1) amortised time.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* Cached values, indexed from 1 */
static size_t cache_size = 0;  /* Largest index computed so far */
static size_t cache_cap  = 0;  /* Allocated capacity */

/* Ensure storage for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t new_cap = cache_cap;
    if (new_cap == 0) {
        /* initial capacity: up to max(n, 4) + 1 (for index 0 unused) */
        new_cap = (n > 4 ? n+1 : 5);
    }
    while (new_cap <= (size_t)n) {
        new_cap = new_cap * 2;
    }
    if (new_cap != cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort */
            abort();
        }
        cache       = tmp;
        cache_cap   = new_cap;
    }
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for the sequence; return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Allocate space if needed. */
    if (n >= cache_cap) {
        ensure_capacity(n);
    }

    /* Initialise base cases if not yet done. */
    if (cache_size == 0) {
        /* Zero index unused; we reserve it to keep indices matching. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)43;
        cache[4] = (seq_t)463;
        cache_size = 4;
    } else if (cache_size < 4) {
        /* In case this function is called with n<5 before cache filled. */
        if (cache_size < 1) cache[1] = (seq_t)1;
        if (cache_size < 2) cache[2] = (seq_t)5;
        if (cache_size < 3) cache[3] = (seq_t)43;
        if (cache_size < 4) cache[4] = (seq_t)463;
        cache_size = 4;
    }

    /* Grow the cache up to n if necessary. */
    if ((size_t)n > cache_size) {
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)12 * cache[i - 1]
                     - (seq_t)6 * cache[i - 2]
                     - (seq_t)20 * cache[i - 3]
                     - (seq_t)5 * cache[i - 4];
        }
        cache_size = (size_t)n;
    }

    return cache[n];
}
