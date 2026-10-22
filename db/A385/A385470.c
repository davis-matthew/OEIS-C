/*
 * A385470  Expansion of e.g.f. 1/(1 - 2 * arctanh(x)).
 * https://oeis.org/A385470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{odd t=1..n} [ a(n-t) * 2 * C(n, t) * (t-1)! ]
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   zero_new_elements
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(n,t) is binomial coefficient.  The recurrence uses only + and *.
   Implementation uses memoisation; values are stored in a static array and
   grown as needed. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/zero_new_elements.h"

static seq_t *a_vals = NULL;          /* a[n] values */
static seq_t *binom  = NULL;          /* binomial coefficients for current n: C(n, t) */
static seq_t *fact   = NULL;          /* factorials: fact[k] = k! */
static size_t capacity = 0;           /* allocated capacity of the arrays   */
static idx_t  next_index = 0;         /* next index to compute              */

/* Initialise static arrays on first call. */
static void ensure_initialised(idx_t n)
{
    if (next_index != 0) return;            /* already initialised */
    capacity = 1;
    a_vals = (seq_t *)calloc(capacity, sizeof(seq_t));
    binom  = (seq_t *)calloc(capacity, sizeof(seq_t));
    fact   = (seq_t *)calloc(capacity, sizeof(seq_t));
    /* base values */
    a_vals[0] = (seq_t)1;    /* a(0) = 1 */
    binom[0]  = (seq_t)1;    /* binom[0] = C(0,0) = 1 */
    fact[0]   = (seq_t)1;    /* 0! = 1 */
    next_index = 1;          /* next value to compute is a(1) */
}

/* Grow arrays if necessary to hold index n. */
static void grow_arrays_if_needed(idx_t n)
{
    if ((size_t)n < capacity) return;
    size_t old_cap = capacity;
    capacity = capacity == 0 ? 1 : capacity * 2;
    while ((size_t)n >= capacity)
        capacity *= 2;
    a_vals = (seq_t *)realloc(a_vals, capacity * sizeof(seq_t));
    binom  = (seq_t *)realloc(binom,  capacity * sizeof(seq_t));
    fact   = (seq_t *)realloc(fact,   capacity * sizeof(seq_t));
    zero_new_elements(a_vals, old_cap, capacity);
    zero_new_elements(binom,  old_cap, capacity);
    zero_new_elements(fact,   old_cap, capacity);
    /* keep binom[0] = 1 */
    binom[0] = (seq_t)1;
}

/* Compute a(n) for all indices up to n, using memoisation. */
static void compute_up_to(idx_t n)
{
    for (; next_index <= n; ++next_index) {
        idx_t cur = next_index;        /* compute a(cur) */
        if (cur == 0) continue;       /* a(0) already set */

        grow_arrays_if_needed(cur);

        /* Update binomial coefficients C(cur, t) using Pascal's rule:
           C(cur, t) = C(cur-1, t) + C(cur-1, t-1) */
        for (idx_t t = cur; t > 0; --t) {
            binom[t] = binom[t] + binom[t - 1];
        }
        /* binom[0] stays 1 automatically */

        /* factorial */
        fact[cur] = fact[cur - 1] * (seq_t)cur;

        /* recurrence for a(cur) */
        seq_t sum = 0;               /* initialise to zero (works for all seq_t) */
        for (idx_t t = 1; t <= cur; t += 2) {  /* only odd t */
            seq_t term = a_vals[cur - t];          /* a(cur-t) */
            term = term * ((seq_t)2 * binom[t]);   /* * 2 * C(cur, t) */
            term = term * fact[t - 1];            /* * (t-1)! */
            sum   = sum + term;                   /* accumulate */
        }
        a_vals[cur] = sum;            /* store the result */
    }
}

/* Main function required by the benchmark. */
seq_t a(idx_t n)
{
    ensure_initialised(n);
    compute_up_to(n);
    return a_vals[n];
}
