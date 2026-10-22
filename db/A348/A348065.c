/*
 * A348065  Coefficient of x^4 in expansion of n!* Sum_{k=0..n} binomial(x,k).
 * https://oeis.org/A348065
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * Stirling1(k,4) * (n-k)!
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes factorials and signed Stirling numbers
   of the first kind for k ≤ 4. It uses the helper `binomial(n,k)` from
   the utils library and performs the sum in O(n) time per call. */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides binomial(idx_t, idx_t) -> seq_t */

static idx_t max_prepared = 0;
static seq_t *fact   = NULL;
static seq_t *s0 = NULL;   /* s(n,0) */
static seq_t *s1 = NULL;   /* s(n,1) */
static seq_t *s2 = NULL;   /* s(n,2) */
static seq_t *s3 = NULL;   /* s(n,3) */
static seq_t *s4 = NULL;   /* s(n,4) */

/* Ensure that all tables are prepared up to `n`.  The arrays are grown
   only as needed and updated incrementally. */
static void prepare(idx_t n)
{
    if (n <= max_prepared)
        return;

    /* Allocate or grow arrays to hold indices 0 .. n. */
    size_t sz = n + 1;
    fact = (seq_t *)realloc(fact, sz * sizeof(seq_t));
    s0   = (seq_t *)realloc(s0,   sz * sizeof(seq_t));
    s1   = (seq_t *)realloc(s1,   sz * sizeof(seq_t));
    s2   = (seq_t *)realloc(s2,   sz * sizeof(seq_t));
    s3   = (seq_t *)realloc(s3,   sz * sizeof(seq_t));
    s4   = (seq_t *)realloc(s4,   sz * sizeof(seq_t));

    if (max_prepared == 0) {
        /* Base case n = 0 */
        fact[0] = (seq_t)1;
        s0[0]   = 1;
        s1[0]   = 0;
        s2[0]   = 0;
        s3[0]   = 0;
        s4[0]   = 0;
    }

    /* Compute new rows using the standard recurrence for Stirling
       numbers of the first kind (signed). */
    for (idx_t i = max_prepared + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;   /* n! = (n-1)! * n */

        s0[i] = 0;  /* s(n,0)=0 for n>0 */

        s1[i] = s0[i - 1] - (seq_t)(i - 1) * s1[i - 1];
        s2[i] = s1[i - 1] - (seq_t)(i - 1) * s2[i - 1];
        s3[i] = s2[i - 1] - (seq_t)(i - 1) * s3[i - 1];
        s4[i] = s3[i - 1] - (seq_t)(i - 1) * s4[i - 1];
    }

    max_prepared = n;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;     /* values below the offset are unspecified */

    prepare(n);

    seq_t sum = 0;
    /* Start with C(n,4) and update it iteratively. */
    seq_t binom = binomial(n, 4);

    for (idx_t k = 4; k <= n; ++k) {
        seq_t term = binom;
        term = term * s4[k];           /* C(n,k) * Stirling1(k,4) */
        term = term * fact[n - k];     /* * (n-k)! */
        sum += term;

        if (k < n) {
            /* binom(n,k+1) = binom(n,k) * (n-k) / (k+1) */
            seq_t num = n - k;
            seq_t den = k + 1;
            binom = binom * num / den;
        }
    }

    return sum;
}
