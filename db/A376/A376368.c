/*
 * A376368  Least number k with a partition k = x_1 + ... + x_j such that the
 *          multinomial coefficient k!/(x_1! * ... * x_j!) is equal to n.
 * https://oeis.org/A376368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n is a multinomial coefficient of k
 *            (i.e., n = k!/(x1! * … * xj!) for some partition k = x1 + … +
 *            xj)
 * Range:     u8 n <= 33, u16 n <= 1625, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/int_sqrt.h"/* idx_t int_sqrt(idx_t n); */

/* ------------------------------------------------------------------ */
/* Direct cache for repeated calls (works for both C and C++).        */
/* Size chosen to comfortably hold all test values in the problem    */
/* (x <= 200000).  For larger n, the function will recompute.        */
#define CACHE_MAX   200000

static seq_t cache[CACHE_MAX + 1] = { 0 };   /* 0 means “unknown” */

/* ------------------------------------------------------------------ */
/* Recursive search: does a partition of “rem” (with parts ≤ last)
   produce the multinomial coefficient equal to target_val?           */
static bool has_partition(idx_t rem, idx_t last, seq_t coeff, seq_t target_val)
{
    for (idx_t p = 1; p <= last && p <= rem; ++p) {
        /* binom(rem,p) is the factor added when the next part equals p */
        seq_t bin = binom(rem, p);
        seq_t newcoeff = coeff * bin;
        if (newcoeff > target_val)
            continue;                   /* prune – already too big */

        if (p == rem) {                 /* finished a partition   */
            if (newcoeff == target_val)
                return true;
        } else {
            /* next part cannot exceed current part to keep non‑increasing order */
            idx_t next_last = (p <= rem - p) ? p : rem - p;
            if (has_partition(rem - p, next_last, newcoeff, target_val))
                return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.                         */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                /* a(1) = 0 by definition */

    if (n <= CACHE_MAX && cache[(idx_t)n] != 0)
        return cache[(idx_t)n];

    /* Target value as a seq_t for all comparisons. */
    seq_t target_val = (seq_t)n;

    /* Upper bound on k that can yield a coefficient ≤ n with more than one part.
       For a partition with at least two parts, the smallest non‑trivial coefficient
       is C(k,1)=k.  Any larger k would give coefficients of the order k or
       larger, which already exceeds n for k > √(2n). */
    idx_t limit = int_sqrt(n + n) + 2;   /* floor sqrt(2*n) + 2  */
    if (limit > n)
        limit = n;

    idx_t ans = n;                       /* fallback: (n-1,1) gives coefficient n */
    for (idx_t k = 2; k <= limit; ++k) {
        if (has_partition(k, k, 1, target_val)) {
            ans = k;
            break;                       /* first (smallest) k found */
        }
    }

    /* Cache the result for future calls (only for indices we can store). */
    if (n <= CACHE_MAX)
        cache[(idx_t)n] = (seq_t)ans;

    return (seq_t)ans;
}
