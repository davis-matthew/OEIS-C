/*
 * A360077  Odd numbers k such that k mod (k-s) = 1, where s is the greatest
 *          square < k.
 * https://oeis.org/A360077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360077: odd numbers k such that k mod (k – s) = 1,
   where s is the greatest square < k. This implementation
   builds the sequence incrementally and memoises the results. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;   /* memoised terms */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */
static seq_t next_k = (seq_t)3; /* next odd number to test */

/* Ensure the cache has at least `need` slots. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    /* cast result of realloc so it compiles in C++ with non‑trivial seq_t */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort(); /* out of memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, return 0 for safety */

    /* Build cache up to index n */
    while (cache_len < n) {
        /* greatest square < next_k */
        seq_t r = floor_sqrt(next_k - (seq_t)1);
        seq_t s = r * r;
        seq_t q = next_k - s;

        if (next_k % q == (seq_t)1) {
            /* found a term; add to cache */
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_k;
        }
        /* advance to the next odd integer */
        next_k += (seq_t)2;
    }

    return cache[n - 1];
}
