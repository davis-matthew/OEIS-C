/*
 * A308849  Expansion of e.g.f. 1 / (BesselI(0,2*x) + BesselI(1,2*x)).
 * https://oeis.org/A308849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is defined by the reciprocal of the exponential generating
 *            function 1 / (BesselI(0,2x)+BesselI(1,2x)), i.e. 1 / ( Σ_{k≥0}
 *            binomial(k,⌊k/2⌋) * x^k / k! ). The coefficients satisfy a(0) =
 *            1 a(n) = - Σ_{k=0}^{n-1} binom(n,k) * a(k) *
 *            binomial(n-k,⌊(n-k)/2⌋) for all n ≥ 1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */

/* Cached values of the sequence and of the coefficients c(m)
   = binomial(m, ⌊m/2⌋). */
static seq_t *a_cache = NULL;
static seq_t *c_cache = NULL;
static size_t a_cap = 0;   /* current number of allocated a values          */
static size_t c_cap = 0;   /* current number of allocated c values          */

/* Ensure c_cache contains all c(m) for 0 ≤ m ≤ n. */
static void ensure_c_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (c_cap >= needed)
        return;
    seq_t *new_cache = (seq_t *)realloc(c_cache, needed * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Out of memory – abort. */
        abort();
    }
    c_cache = new_cache;
    for (size_t i = c_cap; i < needed; ++i) {
        idx_t half = (idx_t)i / 2;            /* floor(i/2) */
        c_cache[i] = binomial((idx_t)i, (idx_t)half);
    }
    c_cap = needed;
}

/* Ensure a_cache contains all a(k) for 0 ≤ k ≤ n. */
static void ensure_a_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (a_cap >= needed)
        return;
    seq_t *new_cache = (seq_t *)realloc(a_cache, needed * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Out of memory – abort. */
        abort();
    }
    a_cache = new_cache;
    for (size_t i = a_cap; i < needed; ++i) {
        idx_t idx = (idx_t)i;
        if (idx == 0) {
            a_cache[i] = (seq_t)1;     /* a(0) = 1 */
        } else {
            seq_t sum = (seq_t)0;
            /* binom holds binomial(i,k) with k running from 0 to i-1. */
            seq_t binom = (seq_t)1;     /* binomial(i,0) */
            for (idx_t k = 0; k < idx; ++k) {
                /* term = binom * a(k) * c(i-k) */
                seq_t term = binom * a_cache[(size_t)k] * c_cache[(size_t)(i - k)];
                sum += term;
                /* Update binom to binomial(i,k+1). */
                binom = binom * (seq_t)(idx - k) / (seq_t)(k + 1);
            }
            a_cache[i] = -sum;
        }
    }
    a_cap = needed;
}

seq_t a(idx_t n)
{
    /* The recurrence requires c(m) up to m = n and a(k) up to k = n. */
    ensure_c_cache(n);
    ensure_a_cache(n);
    return a_cache[(size_t)n];
}
