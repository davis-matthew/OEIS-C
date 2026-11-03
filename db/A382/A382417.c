/*
 * A382417  Numbers with at least one zero in their base-8 representation.
 * https://oeis.org/A382417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8590 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th nonnegative integer that has at least one
   zero digit in its base‑8 representation (OEIS A382417). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if v (in base 8) contains at least one zero digit. */
static bool has_zero_oct(idx_t v)
{
    if (v == 0)                 /* 0 itself has a zero digit */
        return true;
    while (v != 0) {
        if ((v % (idx_t)8) == 0)
            return true;        /* found a 0 digit */
        v /= (idx_t)8;          /* move to next octal digit */
    }
    return false;               /* no zero digit found */
}

/* Static memoisation storage --------------------------------------------------- */
/* Array of computed terms.  The array is grown as needed. */
static seq_t *cache = NULL;      /* allocated cache */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current capacity of the array */

/* Next integer to examine for a zero octal digit.  It is held in a
   type wide enough to grow beyond seq_t. */
static idx_t next_candidate = 0;

/* Grow the cache to accommodate at least ‘needed’ elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;                 /* already large enough */

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    if (!new_ptr)               /* allocation failure – abort */
        abort();

    cache = new_ptr;
    cache_cap = new_cap;
}

/* Public sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)                   /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    size_t target = (size_t)n;   /* at least ‘n’ terms are needed */

    if (target > cache_len) {    /* generate missing terms */
        grow_cache(target);
        while (cache_len < target) {
            /* Find the next integer that contains a zero in base‑8 */
            while (!has_zero_oct(next_candidate))
                ++next_candidate;

            /* Store it as the next term in the cache */
            cache[cache_len++] = (seq_t)next_candidate;
            ++next_candidate;    /* advance for the next search */
        }
    }

    return cache[n - 1];          /* offset 1: a(1) corresponds to cache[0] */
}
