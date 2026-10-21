/*
 * A283682  Unique sequence with a(1)=0, a(2)=1, representing an array T(i,j)
 *          read by antidiagonals in which T(i,j) = a(i) + a(j).
 * https://oeis.org/A283682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9625, u16 n <= 9625, u32 n <= 9625, u64 n <= 9625,
 *            u128 none, big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is defined recursively by  
   a(1)=0, a(2)=1 and for n>2 let (i,j) be the coordinates of n on the
   antidiagonal ordering (i+j = d+1). Then a(n)=a(i)+a(j).  The
   following uses the utility functions seq_alloc_new_array and
   free_seq_array for proper construction/destruction of seq_t values. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Grow the cache to hold at least n terms (n is 1‑based).  Returns
   the array pointer with first cache_len elements initialized. */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_len)
        return;

    /* Allocate new storage large enough.  The utility allocates
       already-constructed objects, so we can assign to them. */
    const size_t new_cap = (size_t)n;
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    if (cache)
        free_seq_array(cache);
    cache = new_cache;
    cache_cap = new_cap;

    /* Initialise base values if cache was empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;          /* a(1) */
        cache_len = 1;
        /* a(2) will be set in the loop below if n>1. */
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)1;          /* a(2) */
        cache_len = 2;
    }

    /* Compute any new terms up to requested n. */
    while (cache_len < n) {
        idx_t k = (idx_t)(cache_len + 1);     /* 1‑based index to compute */

        /* Find the diagonal d such that d*(d+1)/2 >= k.
           Diagonal 1 has length 1, diagonal 2 has length 2, … */
        idx_t d = 1;
        while (d * (d + 1) / 2 < k)
            ++d;

        /* Position inside the diagonal (1‑based). */
        idx_t pos = k - (d - 1) * d / 2;

        /* Coordinates: i = pos, j = d+1-pos (both 1‑based). */
        idx_t i = pos;
        idx_t j = d + 1 - pos;

        /* a(k) = a(i) + a(j). Convert to 0‑based indices. */
        seq_t val = cache[i - 1] + cache[j - 1];
        cache[cache_len] = val;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1; return 0 for safety */
        return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
