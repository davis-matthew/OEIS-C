/*
 * A260375  Numbers k such that A260374(k) is a perfect square.
 * https://oeis.org/A260375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A260375 – Numbers k such that A260374(k) is a perfect square
 *
 * The sequence consists of all non‑negative integers k for which
 * A260374(k) = |k! – nearest integer square| is itself a perfect
 * square.  The returned value a(n) is the n‑th element of that list
 * (OEIS indexes start at 1, so a(1) = 0).
 *
 * Implementation notes:
 *   •  The function builds a dynamic cache of the required values.
 *   •  Factorials are generated incrementally; each new k is tested
 *       exactly once.
 *   •  floor_sqrt (provided by the utils) is used to find the nearest
 *       square and to test whether a difference is a square.
 *   •  The routine works for any integer type supplied by the build
 *       (uint8_t … uint64_t, unsigned __int128, or an arbitrary‑precision
 *       class).  The only external dependency required is
 *       floor_sqrt, whose header is included.
 *   •  No global state other than the cache is altered by repeated
 *       calls, and the sequence can be requested in any order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Cache of the sequence values (k's).  cache[0] holds a(1). */
static seq_t *cache = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;   /* number of values stored */

/* Current factorial state: fact == (next_k)!  */
static seq_t   fact    = 1;
static idx_t   next_k  = 0;     /* next integer to process (starting at 0) */

/* Ensure the cache can store at least `need` elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(EXIT_FAILURE);   /* premature exit on OOM */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the next sequence value and append it to the cache. */
static void append_candidate(void)
{
    /* test current k = next_k, with fact = k! */
    seq_t sq_root = floor_sqrt(fact);
    seq_t sq1 = sq_root * sq_root;
    seq_t d1 = fact - sq1;          /* distance to the lower square */
    seq_t sq2 = (sq_root + 1) * (sq_root + 1);
    seq_t d2 = sq2 - fact;          /* distance to the upper square */
    seq_t d = d1 < d2 ? d1 : d2;    /* nearest distance */

    seq_t root_d = floor_sqrt(d);
    if (root_d * root_d == d) {
        /* d is a perfect square, so next_k qualifies */
        grow_cache(cache_len + 1);
        cache[cache_len++] = next_k;
    }

    /* advance to (k+1)! for the next iteration */
    next_k++;
    fact *= (seq_t)next_k;
}

/* Make sure the cache holds at least `n` elements (i.e. at least a(n)). */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        append_candidate();
    }
}

/* Required API. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0; guard anyway */
    ensure_cache(n);
    return cache[n - 1];
}
