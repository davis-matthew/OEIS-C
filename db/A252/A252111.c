/*
 * A252111  Number of (n+2) X (5+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum equal to 0 2 3 6 or 7 and every 3 X 3 diagonal and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 26, u64 n <= 57, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A252111
    Number of (n+2) X (5+2) 0..3 arrays with the given row/column sum
    restriction and diagonal/antidiagonal sum restriction.
    Recurrence (empirical):  a(n) = 12*a(n-3) - 32*a(n-6)   for n > 11
    Initial values:
        a(1) = 1884   a(2) = 624   a(3) = 1258
        a(4) = 1956   a(5) = 3528   a(6) = 6070
        a(7) = 11760  a(8) = 21344  a(9) = 39200
        a(10)= 80832  a(11)=147840  a(12)=276160
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* 1‑based cache of already computed terms */
static size_t cache_size = 0;    /* current size of cache (cache[i] valid for 0 ≤ i < cache_size) */

/* Initial values (index 0 unused) */
static const seq_t init_vals[13] = {
    0ULL,          /* (unused) */
    1884ULL,       /* a(1) */
    624ULL,        /* a(2) */
    1258ULL,       /* a(3) */
    1956ULL,       /* a(4) */
    3528ULL,       /* a(5) */
    6070ULL,       /* a(6) */
    11760ULL,      /* a(7) */
    21344ULL,      /* a(8) */
    39200ULL,      /* a(9) */
    80832ULL,      /* a(10) */
    147840ULL,     /* a(11) */
    276160ULL      /* a(12) */
};

seq_t a(idx_t n)
{
    /* NOTE: the sequence is defined for n >= 1; for n == 0 return 0. */
    if (n == 0) return 0;

    /* delegate to size_t for array indexing; safe for the n values we handle */
    size_t idx = (size_t)n;

    /* Ensure cache is large enough and compute any missing terms */
    if (idx >= cache_size) {
        size_t oldsize = cache_size;
        size_t newsize = idx + 1;              /* need indices 0 .. idx inclusive */

        /* Resize the cache */
        seq_t *newptr = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newptr) abort();                  /* allocation failure */
        cache = newptr;
        /* Initialise newly allocated slots (they may contain garbage) */
        for (size_t i = oldsize; i < newsize; ++i) cache[i] = 0;
        cache_size = newsize;

        /* Compute values that are not yet in the cache */
        size_t start = oldsize > 1 ? oldsize : 1;   /* indices to start computing from */
        for (size_t i = start; i <= idx; ++i) {
            seq_t val;
            if (i <= 12) {
                val = init_vals[(size_t)i];
            } else {
                /* Recurrence: a(n) = 12*a(n-3) - 32*a(n-6) */
                val = 12 * cache[i - 3] - 32 * cache[i - 6];
            }
            cache[i] = val;
        }
    }

    return cache[idx];
}
