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
 * Validated: standard, pass, 98 terms through n = 9880 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Convert an integer into its factorial base digits and
 * return true iff any digit equals 1. */
static bool has_one_digit_factorial_base(seq_t m)
{
    if (m == (seq_t)0)
        return false;
    seq_t t = m;
    idx_t i = 1;                /* base = i+1 (2,3,4,...) */
    while (t > (seq_t)0) {
        seq_t d = t % (seq_t)(i + 1);
        if (d == (seq_t)1)
            return true;
        t = t / (seq_t)(i + 1);
        ++i;
    }
    return false;
}

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
        cache = mem_new(seq_t, cache_cap);
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
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
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
