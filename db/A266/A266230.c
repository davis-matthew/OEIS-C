/*
 * A266230  Least positive integer x such that n + x^2 = y^3 + z^3 for some
 *          positive integers y and z, or 0 if no such x exists.
 * https://oeis.org/A266230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9066 (2026-11-03)
 * Helpers:   icbrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A266230:
   a(n) = least positive integer x such that
          n + x^2 = y^3 + z^3 for some positive integers y, z;
          return 0 if no such x exists. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/icbrt_seq.h"
#include "../utils/mem.h"

/* Compute a single a(n) by brute force search.  This function is
   intentionally not exported; it is used by the caching routine below. */
static seq_t compute_a_by_search(idx_t n)
{
    /* Search x from 1 upward until a representation is found. */
    for (idx_t x = 1;; ++x) {
        seq_t xsq   = (seq_t)x * (seq_t)x;
        seq_t target = (seq_t)n + xsq;

        /* The largest cube root that can appear in a solution for this target. */
        seq_t y_max = icbrt_seq(target);
        if (y_max == 0)
            continue;   /* target is 0; impossible for positive y */

        /* Two‑pointer search over cubes y^3 + z^3 = target. */
        seq_t y = 1;
        seq_t z = y_max;
        while (y <= z) {
            seq_t y3 = y * y * y;
            seq_t z3 = z * z * z;
            seq_t sum = y3 + z3;

            if (sum == target)
                return (seq_t)x;          /* minimal x found */
            if (sum < target)
                ++y;
            else
                --z;
        }
    }
}

/* Cache for already computed values of a(n).  The cache grows on demand
   and is indexed by the integer n.  The cache content is persistent
   across calls, so repeated requests for the same n are answered in O(1). */
static seq_t *a_cache = NULL;      /* array of a(n) values */
static idx_t  cache_size = 0;      /* how many entries are allocated */

/* Ensure that the cache is large enough to hold entries up to and including n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = n + 1;
    seq_t *new_cache = mem_resize(seq_t, a_cache, new_size);
    if (new_cache == NULL) {
        /* Out of memory – fall back to computing the value on the fly. */
        return;
    }
    /* Initialize new entries to 0 (meaning "not yet computed") */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
    a_cache   = new_cache;
    cache_size = new_size;
}

/* Public function required by the tests. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* If the value is not yet computed, compute it and store. */
    if (a_cache[n] == 0) {
        a_cache[n] = compute_a_by_search(n);
    }
    return a_cache[n];
}
