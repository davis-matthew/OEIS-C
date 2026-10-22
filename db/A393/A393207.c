/*
 * A393207  Number of compositions (c1, c2, ...) of n with each c_i <=
 *          phi*c_{i-1}, where phi = (1 + sqrt(5))/2 = A001622.
 * https://oeis.org/A393207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 58, u64 n <= 119,
 *            u128 n <= 242, big any n
 * Helpers:   pref_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A393207: Number of compositions of n with each part
 * ≤ φ times the preceding part (φ = (1+√5)/2).
 *
 * Recurrence used:
 *   f(s,p) = 1                         if s == p
 *   f(s,p) = Σ_{k=1..min(s-p,⌊φ·p⌋)} f(s-p,k)   otherwise
 *   a(s)   = Σ_{p=1..s} f(s,p)
 *
 * To evaluate the sum efficiently we store prefix sums
 *   pref[s][p] = Σ_{k=1..p} f(s,k)
 * so that for s>p:
 *   f(s,p) = pref[s-p][ min(s-p,⌊φ·p⌋) ].
 *
 * pref[s][0] is defined as 0 for all s.  The table is built
 * incrementally and reused for any order of calls.
 * φ·p is rounded down via the identity
 *   ⌊φ·p⌋ = (p + ⌊√5·p⌋)/2
 * where √5·p is computed with integer square‑root.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pref_index.h"

/* Integer square‑root for 64‑bit unsigned numbers */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long low = 0, high = 0xFFFFFFFFULL;
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        unsigned long long sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Cached data */
static size_t  max_n   = 0;   /* maximum n already computed */
static seq_t * pref    = NULL; /* flattened triangular array of prefix sums */
static idx_t * floor_phi = NULL; /* ⌊φ · p⌋ for 0 ≤ p ≤ max_n */

/* Extend the cache so that a(n) is available for all n ≤ upto */
static void ensure_n(idx_t upto)
{
    if (upto <= max_n) return;

    /* Reallocate pref array */
    size_t new_pref_size = ((size_t)upto + 1) * ((size_t)upto + 2) / 2;
    pref = (seq_t *)realloc(pref, new_pref_size * sizeof(seq_t));

    /* Reallocate floor_phi array */
    floor_phi = (idx_t *)realloc(floor_phi,
                                 ((size_t)upto + 1) * sizeof(idx_t));

    /* Compute ⌊φ·p⌋ for new p values */
    for (size_t p = max_n + 1; p <= upto; ++p) {
        unsigned long long sq5 = 5ULL * (unsigned long long)p * (unsigned long long)p;
        unsigned long long sqrt5 = isqrt_u64(sq5);
        floor_phi[p] = (p + sqrt5) / 2;
    }

    /* Build new rows of pref table */
    for (size_t s = max_n + 1; s <= upto; ++s) {
        size_t row_start = pref_index(s, 0);          /* offset of pref[s][0] */
        pref[row_start] = 0;                          /* pref[s][0] = 0 */
        seq_t cumulative = 0;
        for (size_t p = 1; p <= s; ++p) {
            seq_t val;
            if (s == p) {
                val = 1;
            } else {
                size_t rem   = s - p;
                idx_t  limit = floor_phi[p];
                if (limit > rem) limit = rem;
                size_t idx_rem_limit = pref_index(rem, (size_t)limit);
                val = pref[idx_rem_limit];
            }
            cumulative += val;
            pref[row_start + p] = cumulative;
        }
    }

    max_n = upto;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;    /* a(0) = 1 by definition */
    ensure_n(n);
    return pref[pref_index(n, n)];
}
