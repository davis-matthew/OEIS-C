/*
 * A258823  Numbers m such that k iterations of m under the '3x+1' map yield k
 *          for some k.
 * https://oeis.org/A258823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 310, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers m such that ∃k≥1: C^k(m)=k
   where C(x)=x/2 if x even, C(x)=3x+1 if x odd.
   The function a(idx_t n) returns the nth term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_STEPS 40000   /* safety limit for the forward Collatz test */

/* Test if a given number m has the Collatz‑k property. */
static inline bool has_k_equivalent(seq_t m)
{
    seq_t cur = m;
    for (idx_t step = 1; step <= MAX_STEPS; ++step) {
        /* next Collatz step */
        if (cur & 1) { /* odd */
            unsigned __int128 tmp = (unsigned __int128)cur * 3 + 1;
            cur = (seq_t)tmp;          /* cast back to seq_t */
        } else {
            /* shift is allowed for all seq_t types that support it
               (built‑in integers and most arbitrary‑precision classes).   */
            cur >>= 1;
        }

        /* check the equality condition */
        if (cur == (seq_t)step)
            return true;

        /* if we reached 1, the cycle 4→2→1 will never satisfy cur==step
           for larger step values, so we can stop early.                 */
        if (cur == 1)
            return false;
    }
    return false;  /* safety fallback if a very long orbit is found */
}

/* Cached sequence values (1‑based indexing) */
static seq_t *cache      = NULL;   /* array of found terms      */
static size_t cache_size = 0;      /* how many terms are stored */
static size_t cache_cap  = 0;      /* allocated capacity       */
static seq_t next_m      = 2;      /* next integer to test     */

/* Ensure the cache array can hold at least one more element. */
static void ensure_capacity(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr)
            abort();
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Public function implementing OEIS sequence A258823. */
seq_t a(idx_t n)
{
    /* Non‑decreasing calls are handled correctly because the static
       cache grows only when required. */
    while (cache_size < (size_t)n) {
        if (has_k_equivalent(next_m)) {
            ensure_capacity();
            cache[cache_size++] = next_m;
        }
        ++next_m;
    }
    return cache[n - 1];
}
