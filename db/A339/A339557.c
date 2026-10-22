/*
 * A339557  a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-1) - n if a(n) is
 *          nonnegative, not already in the sequence, and gcd(a(n-1),n) > 1 or
 *          gcd(a(n-2),n) = 1. Otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A339557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for A339557 a(0) = 0, a(1) = 1 for n > 1: let p =
 *            a(n-1) let cand = p - n (if p >= n) if cand >= 0 and cand not
 *            seen before and ( gcd(p,n) > 1 or gcd(a(n-2),n) == 1 ) then a(n)
 *            = cand else a(n) = p + n
 * Range:     u8 n <= 46, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

/* static cache of computed terms */
static seq_t *seq   = NULL; /* pointer to array of terms   */
static size_t cache_len = 0;/* number of terms stored      */
static size_t cache_cap = 0;/* capacity of the array       */

/* Ensure the array has space for indices 0..n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;              /* we need at least n+1 slots   */
    if (needed <= cache_cap) return;             /* already enough space          */

    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 64;              /* initial capacity             */
    while (new_cap < needed) new_cap <<= 1;      /* grow by factor 2            */

    /* realloc and cast to seq_t* for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) { abort(); }
    seq   = tmp;
    cache_cap = new_cap;
}

/* Check whether val has been used before index 'limit' (exclusive) */
static bool seen_before(seq_t val, size_t limit)
{
    for (size_t i = 0; i < limit; ++i)
        if (seq[i] == val) return true;
    return false;
}

/* Main function to compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Base terms are handled directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Make sure we have space for n+1 entries */
    ensure_cache(n);

    /* Initialise the cache if it is the first call */
    if (cache_len == 0) {
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t prev   = seq[i-1];
        seq_t cur;

        /* candidate by subtraction */
        if (prev >= (seq_t)i) {
            seq_t cand = prev - (seq_t)i;
            if (!seen_before(cand, (size_t)i)) {
                seq_t g1 = gcd(prev, (seq_t)i);
                seq_t g2 = gcd(seq[i-2], (seq_t)i);
                if ( (g1 > (seq_t)1) || (g2 == (seq_t)1) )
                    cur = cand;
                else
                    cur = prev + (seq_t)i;
            } else
                cur = prev + (seq_t)i;
        } else
            cur = prev + (seq_t)i;

        seq[i] = cur;
    }
    cache_len = (size_t)n + 1;
    return seq[(size_t)n];
}
