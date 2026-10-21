/*
 * A285868  Row sums of triangle A285867.
 * https://oeis.org/A285868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285868: Row sums of triangle A285867
   Formula: a(0) = 1
   a(n) = 2 * Σ_{k=1..n} k! * S(n,k) - n!          (S(n,k) = Stirling numbers of the second kind) */

/* The implementation keeps a running row of Stirling numbers and factorials,
   accumulates the required sum, and caches results for all n <= 10000
   (or larger if needed).  The algorithm is O(n^2) total time but
   per-call complexity is O(1) after precomputation.  All arithmetic is
   performed in seq_t which is guaranteed to be large enough for the
   terms that fit in the target width. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    /* static state */
    static idx_t max_computed = 0;          /* highest n already computed */
    static size_t capacity = 0;             /* allocated array size (>= next up +1) */
    static seq_t *S_prev = NULL;            /* previous row S(n-1, k)   */
    static seq_t *S_curr = NULL;            /* current row  S(n,   k)   */
    static seq_t *fact  = NULL;             /* factorials              */
    static seq_t *a_cache = NULL;           /* cached a(n) values      */

    /* Ensure containers have room for indices up to 'up' (inclusive). */
    if (capacity <= (size_t)n)
    {
        size_t new_cap = (size_t)n * 2 + 16;        /* overshoot to avoid frequent reallocs */

        /* Allocate new arrays.  The utility macro returns raw malloc-ed memory. */
        seq_t *new_prev = seq_alloc_new_array(new_cap);
        seq_t *new_curr = seq_alloc_new_array(new_cap);
        seq_t *new_fact = seq_alloc_new_array(new_cap);
        seq_t *new_a    = seq_alloc_new_array(new_cap);

        /* Copy existing data if any */
        size_t i;
        for (i = 0; i < capacity; ++i)
        {
            new_prev[i] = S_prev ? S_prev[i] : 0;
            new_curr[i] = S_curr ? S_curr[i] : 0;
            new_fact[i] = fact  ? fact[i]  : 0;
            new_a[i]    = a_cache? a_cache[i] : 0;
        }
        /* Zero the rest of the new arrays */
        for (i = capacity; i < new_cap; ++i)
        {
            new_prev[i] = new_curr[i] = new_fact[i] = new_a[i] = 0;
        }

        /* Free old memory if it exists */
        if (S_prev) free(S_prev);
        if (S_curr) free(S_curr);
        if (fact)   free(fact);
        if (a_cache) free(a_cache);

        /* Commit new storage */
        S_prev   = new_prev;
        S_curr   = new_curr;
        fact     = new_fact;
        a_cache  = new_a;
        capacity = new_cap;

        /* Initialize base case if this is the first allocation */
        if (max_computed == 0)
        {
            fact[0]   = 1;      /* 0! = 1 */
            S_prev[0] = 1;      /* S(0,0) = 1 */
            a_cache[0] = 1;     /* a(0) = 1 */
        }
    }

    if (n == 0) return a_cache[0];

    /* Extend precomputation up to n if necessary */
    for (idx_t m = max_computed + 1; m <= n; ++m)
    {
        /* factorial: fact[m] = fact[m-1] * m */
        fact[m] = mul_small(fact[m-1], (unsigned int)m);

        /* Compute S(m,k) from S(m-1,k) */
        S_curr[0] = 0;                          /* S(m,0) = 0 for m>0 */
        seq_t sumRow = 0;                       /* Σ k! * S(m,k) */

        for (idx_t k = 1; k <= m; ++k)
        {
            /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            seq_t term1 = mul_small(S_prev[k], (unsigned int)k);
            seq_t term2 = S_prev[k-1];
            S_curr[k] = term1 + term2;

            /* Accumulate k! * S(m,k) */
            seq_t prod = fact[k] * S_curr[k];
            sumRow += prod;
        }

        /* a(m) = 2 * sumRow - fact[m] */
        a_cache[m] = sumRow * (seq_t)2 - fact[m];

        /* Prepare for next iteration */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }

    max_computed = n;
    return a_cache[n];
}
