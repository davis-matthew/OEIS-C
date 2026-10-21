/*
 * A250051  Numbers n such that m = floor(n/5) is not coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 3089, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250051: Numbers n such that m = floor(n/5) is not coprime to n
 *          and, if m≠0, m is also a term of the sequence.  The function
 *          builds the sequence iteratively in increasing order, caching
 *          terms for O(1) access to a(n) for arbitrary n.
 *
 * Formula: a(n) = nth integer n with m=floor(n/5) satisfying gcd(n,m)>1
 *              and (m==0 or m∈{a(k)}).  (Base 5)
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Cached sequence terms (ascending). */
static seq_t *seq_cache = NULL;
static size_t  seq_len   = 0;   /* number of stored terms */
static size_t  seq_cap   = 0;   /* allocated capacity */

/* Ensure the cache has room for at least 'need' elements. */
static void
ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* Binary search – true if 'value' is already a term. */
static bool
contains(seq_t value)
{
    size_t lo = 0, hi = seq_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t v   = seq_cache[mid];
        if (v < value)
            lo = mid + 1;
        else if (v > value)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* Return the nth term of the sequence, n ≥ 1.  n==0 returns 0. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defensive, not part of OEIS offset */

    /* Grow the sequence until we have at least n terms. */
    while (seq_len < (size_t)n) {
        seq_t cand = (seq_len == 0) ? (seq_t)2
                                   : seq_cache[seq_len - 1] + (seq_t)1;

        /* Find the next term satisfying the RTNC(b=5) property. */
        for (;;) {
            seq_t m = cand / (seq_t)5;          /* floor(cand/5) */
            seq_t g = gcd(cand, m);
            if (g > (seq_t)1 && (m == (seq_t)0 || contains(m))) {
                ensure_capacity(seq_len + 1);
                seq_cache[seq_len++] = cand;
                break;
            }
            cand = cand + (seq_t)1;             /* try next candidate */
        }
    }
    return seq_cache[n - 1];
}
