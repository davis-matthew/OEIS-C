/*
 * A296907  Numbers whose base-60 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8856, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _has_more_pits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A296907, numbers whose base‑60 digits have more pits than peaks.
   Computed by iterating over natural numbers and testing the condition. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_has_more_pits.h"

/* static storage for memoisation */
static seq_t *cache = NULL;     /* array of found terms */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* capacity of the array */
static seq_t last_tested = 3600;/* last number that was examined */
static bool initialized = false;

/* grow the cache if necessary */
static void _ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In a contest environment we can ignore realloc failure as the
       problem guarantees small n. */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        cache = NULL;
        cache_size = 0;
        cache_cap = 0;
        last_tested = 3600;          /* first candidate is 3601 */
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but return 0 safely */

    /* generate terms until we have the n‑th term */
    while (cache_size < (size_t)n) {
        seq_t candidate = ++last_tested;
        if (_has_more_pits(candidate)) {
            _ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = candidate;
        }
    }

    return cache[(size_t)n - 1];
}
