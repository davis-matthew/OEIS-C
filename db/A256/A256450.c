/*
 * A256450  Numbers that have at least one 1-digit in their factorial base
 *          representation (A007623).
 * https://oeis.org/A256450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 228, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_one_digit_factorial_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A256450: numbers that have at least one 1‑digit in their
 *  factorial base representation (A007623).
 *  The implementation enumerates integers and keeps those whose
 *  factorial base representation contains the digit 1
 *  (anywhere).  It memoises already computed values for fast repeated
 *  calls.  No external libraries are required.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_one_digit_factorial_base.h"

/* Cache of already computed sequence values.  The cache grows
 * as needed to answer queries in any order. */
static seq_t *cache            = NULL;
static size_t cache_cap        = 0;   /* allocated capacity */
static idx_t  cache_len        = 0;   /* number of stored terms   */
static seq_t  next_candidate   = 2;   /* next integer to test     */
static bool   cache_ready      = false;

/* Ensure that cache_len > n so that a(n) can be returned. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                      /* already computed */

    if (!cache_ready) {              /* first initialisation   */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();                 /* allocation failure     */
        cache[0] = (seq_t)1;         /* a(0) = 1               */
        cache_len = 1;
        cache_ready = true;
    }

    while (cache_len <= n) {
        /* Find the next integer that satisfies the property. */
        while (!has_one_digit_factorial_base(next_candidate))
            ++next_candidate;

        /* Grow cache if necessary. */
        if ((size_t)cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp)
                abort();             /* allocation failure     */
            cache = tmp;
        }

        /* Store the qualifying integer. */
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
