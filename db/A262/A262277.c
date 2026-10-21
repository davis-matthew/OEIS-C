/*
 * A262277  Numbers having in decimal representation the same distinct decimal
 *          digits as their 9's complement.
 * https://oeis.org/A262277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1757, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, valid_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements OEIS A262277.
    A number is in the sequence when the set of its distinct decimal digits
    is equal to the set of distinct digits of its 9's complement.
    The implementation generates the terms sequentially, caching them
    so that future calls are O(1) except for the additional terms that
    are required.  */

#include <stddef.h>
#include <stdbool.h>

/*  External types supplied by the build  */
#pragma once

/*  The helper used to grow the cache  */
#include "../utils/append_candidate.h"
#include "../utils/valid_value.h"

/*  Cache of sequence values (1‑based offset: cache[0] = a(1))  */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;   /* number of cached values   */
static size_t cache_cap   = 0;   /* current capacity of cache */

/*  Next candidate number to test  */
static seq_t next_candidate = 10;   /* the sequence first term is 18, so start at 10 */

/*  The sequence function requested by the OEIS  */
seq_t a(idx_t n)
{
    /*  Create more terms until we have at least n of them  */
    while (cache_len < (size_t)n) {
        if (valid_value(next_candidate))
            append_candidate(&cache, &cache_len, &cache_cap, next_candidate);
        ++next_candidate;
    }
    /*  idx_t is unsigned: a(1) is stored at cache[0]  */
    return cache[n - 1];
}
