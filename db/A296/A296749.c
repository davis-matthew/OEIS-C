/*
 * A296749  Numbers whose base-12 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fewer_rises_than_falls
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A296749: Numbers whose base‑12 digits have fewer rises than falls.
 * The function a(n) returns the nth such number (a(1)=12, a(2)=24, …).
 * It is computed by straightforward enumeration with a memoising cache.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>   /* malloc/realloc */
#include "../utils/fewer_rises_than_falls.h"

/* Memoised sequence array.  All indices are 1‑based. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms stored           */
static size_t cache_cap  = 0;   /* current capacity (in terms)     */
static seq_t next_candidate = 12;  /* first value to test            */

/* Append a term to the cache, growing the array if necessary. */
static void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* On a failure realloc will return NULL and leave cache unchanged.
           For the purposes of this problem a failure is not expected. */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---- public entry point ---- */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* OEIS offset is 1; for n==0 we return 0 as a safe sentinel. */
        return (seq_t)0;
    }

    /* Enumerate until we have at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        if (fewer_rises_than_falls(next_candidate)) {
            push_cache(next_candidate);
        }
        /* Increment next_candidate; works for integer or big‑int types. */
        next_candidate = next_candidate + 1;
    }
    return cache[n - 1];
}
