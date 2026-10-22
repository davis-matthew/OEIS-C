/*
 * A383051  a(n) is the n-th term of the inverse Stirling transform of j->
 *          (j+1)^n.
 * https://oeis.org/A383051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A383051
    Formula: a(n) = Sum_{k=0..n} (k+1)^n * Stirling1(n,k)
    (Stirling1 are signed Stirling numbers of the first kind)
    The routine keeps a cache so the sequence can be queried in any
    order, but computing up to n uses the standard recurrence
    S(n,k)=S(n-1,k-1)-(n-1)S(n-1,k).  Powers (k+1)^n are updated
    multiplicatively from the previous value */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache  = NULL;   /* a[0..max_computed]               */
static seq_t *s_prev = NULL;   /* Stirling1 for n-1 (size up to cap)*/
static seq_t *s_curr = NULL;   /* temporary row for n              */
static seq_t *pow_arr = NULL;  /* (k+1)^n for current n           */
static idx_t array_cap = 0;    /* current capacity of the arrays   */
static idx_t max_computed = (idx_t)-1;  /* highest n computed */

/* ensure all tables have room for indexes 0..needed */
static void ensure_arrays(idx_t needed)
{
    if (array_cap <= needed) {
        idx_t new_cap = needed + 1;
        cache   = (seq_t*)realloc(cache,   sizeof(seq_t) * (size_t)new_cap);
        s_prev  = (seq_t*)realloc(s_prev,  sizeof(seq_t) * (size_t)new_cap);
        s_curr  = (seq_t*)realloc(s_curr,  sizeof(seq_t) * (size_t)new_cap);
        pow_arr = (seq_t*)realloc(pow_arr, sizeof(seq_t) * (size_t)new_cap);
        array_cap = new_cap;
    }
}

/* compute a(n)   (may be called in any order) */
seq_t a(idx_t n)
{
    /* initialise on the first call */
    if (max_computed == (idx_t)-1) {
        ensure_arrays(0);
        s_prev[0]   = (seq_t)1;   /* Stirling1(0,0) = 1 */
        pow_arr[0]  = (seq_t)1;   /* (0+1)^0 = 1 */
        cache[0]    = (seq_t)1;   /* a(0) = 1 */
        max_computed = 0;
    }

    if (n <= max_computed) return cache[n];

    ensure_arrays(n);

    /* grow from max_computed+1 up to n */
    for (idx_t new_n = max_computed + 1; new_n <= n; ++new_n) {
        /* update existing powers: (k+1)^{new_n} = (k+1)^{new_n-1} * (k+1) */
        for (idx_t k = 0; k < new_n; ++k)
            pow_arr[k] *= (seq_t)(k + 1);

        /* compute new column: (new_n+1)^{new_n} */
        seq_t pow_val = (seq_t)1;
        seq_t base = (seq_t)(new_n + 1);
        for (idx_t t = 0; t < new_n; ++t) pow_val *= base;
        pow_arr[new_n] = pow_val;

        /* Stirling numbers for row new_n */
        s_curr[0] = (seq_t)0;          /* S(n,0)=0 for n>0 */
        seq_t mult = (seq_t)(new_n - 1);
        for (idx_t k = 1; k < new_n; ++k)
            s_curr[k] = s_prev[k - 1] - mult * s_prev[k];
        s_curr[new_n] = (seq_t)1;       /* S(n,n)=1 */

        /* accumulate a(new_n) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= new_n; ++k)
            sum += s_curr[k] * pow_arr[k];
        cache[new_n] = sum;

        /* swap rows for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    max_computed = n;
    return cache[n];
}
