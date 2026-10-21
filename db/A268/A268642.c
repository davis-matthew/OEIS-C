/*
 * A268642  Seelmann's sequence: a(1) = 1; thereafter a(n + 1) =
 *          ceiling(a(n)/2) unless this is already in the sequence, in which
 *          case a(n + 1) = 3*a(n).
 * https://oeis.org/A268642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 2912, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seelmann's sequence (OEIS A268642)
   Recurrence:
       a(1) = 1
       a(n+1) = ceil(a(n)/2)   if that value is not already in the sequence
                = 3*a(n)       otherwise   */

/* This implementation memoises the sequence for arbitrary‑precision
   seq_t values.  It supports calls in any order, grows the cache as
   needed, and uses only integer arithmetic (shifts, +, *, ==). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Cached values of the sequence.  Indexes are zero‑based: cache[0] = a(1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache (in elements) */
static size_t cache_len = 0;   /* number of values currently stored */

/* Grow the cache array to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    /* Allocate new storage. */
    seq_t *new_cache = alloc_seq_array(new_cap);
    /* Copy existing entries. */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    /* Free old storage. */
    free_seq_array(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Linear search: does value val appear among the first len cached terms? */
static bool has_seen(seq_t val, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        if (cache[i] == val)
            return true;
    return false;
}

/* ------------------------------------------------------------------ */
/* Public function ---------------------------------------------------- */
/* Compute the n‑th term (1 ≤ n).  The function is safe for any order
   of calls and for n ≥ 1.  For n == 0 the behavior is undefined; an
   implementation choice is to return 0.                                */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but return a neutral value */

    /* Initialise cache on first invocation. */
    if (!cache) {
        grow_cache(16);
        cache[0] = (seq_t)1;    /* a(1) */
        cache_len = 1;
    }

    /* Grow the cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];

        /* ceil(prev/2) == (prev >> 1) + (prev & 1)   */
        seq_t cand = (prev >> 1) + (prev & (seq_t)1);

        seq_t next = has_seen(cand, cache_len) ? (prev * (seq_t)3) : cand;

        if (cache_len == cache_cap)
            grow_cache(cache_cap * 2 > cache_cap ? cache_cap * 2 : 1);

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
