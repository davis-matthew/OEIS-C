/*
 * A273670  Numbers with at least one maximal digit in their factorial base
 *          representation.
 * https://oeis.org/A273670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_maximal_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A273670.  Numbers with at least one maximal digit in their
 * factorial base representation.
 * Formula: a(0)=1; for n>0, a(n)= the smallest integer >a(n-1) that has a
 * maximal digit in its factorial base representation. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_maximal_digit.h"

/* Cache of terms already computed */
static seq_t *cache = NULL;   /* cache[n] == a(n)                   */
static idx_t  csz   = 0;      /* number of cached terms              */
static idx_t  cap   = 0;      /* allocated capacity of cache          */

/* Grow the cache to at least 'need' entries */
static void grow_cache(idx_t need)
{
    if (need <= cap) return;
    idx_t newcap = cap ? cap << 1 : 32;
    while (newcap < need) newcap <<= 1;
    seq_t *p = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)newcap);
    if (!p) abort();                      /* out of memory */
    cache = p;
    cap   = newcap;
}

/* Ensure that cache[0..n] is filled with a(0)..a(n) */
static void ensure_terms(idx_t n)
{
    if (n < csz) return;                  /* already have enough terms       */

    if (!cache) {                         /* first use – initialise cache   */
        grow_cache(32);
        cache[0] = (seq_t)1;              /* a(0) = 1                        */
        csz = 1;
    }

    while (csz <= n) {                    /* compute missing terms one by one */
        seq_t cand = cache[csz - 1] + 1;
        while (!has_maximal_digit(cand)) cand = cand + 1;
        grow_cache(csz + 1);
        cache[csz++] = cand;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n];
}
