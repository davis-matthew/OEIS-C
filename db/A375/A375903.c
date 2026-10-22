/*
 * A375903  E.g.f. satisfies A(x) = (2 - exp(x * A(x)^(1/3)))^3.
 * https://oeis.org/A375903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A375903:
   a(0) = 1
   a(n) = 3 * (n+2)! * Σ_{k=1..n} (-1)^k * Stirling2(n,k) / (n-k+3)!
   (For n=0 the sum is empty → 1.)
   This implementation builds Stirling numbers of the second kind and
   evaluates the sum in O(n²) time, which suffices for n ≤ 10 000.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Allocate an array of seq_t objects. */
static inline seq_t *alloc_seq_array(size_t n)
{
    return (seq_t *)malloc(sizeof(seq_t) * n);
}

/* Ensure that a dynamic array has at least 'need' elements.
   The array is stored in '*arr', its capacity in '*cap'.
*/
static void ensure_capacity(idx_t need, seq_t **arr, size_t *cap)
{
    if (*cap <= (size_t)need)
    {
        size_t newcap = (*cap > 0) ? *cap : 1;
        while (newcap <= (size_t)need) newcap <<= 1;
        seq_t *newarr = alloc_seq_array(newcap);
        for (size_t i = 0; i < *cap; ++i) newarr[i] = (*arr)[i];
        free(*arr);
        *arr = newarr;
        *cap = newcap;
    }
}

/* Return the n-th term of sequence A375903.  Memoised for performance. */
seq_t a(idx_t n)
{
    /* Static tables for memoisation. */
    static idx_t computed_max = (idx_t)-1;          /* highest n for which a_cache is filled */
    static seq_t *a_cache = NULL;                  /* cached a values */
    static size_t a_cap = 0;
    static seq_t *S = NULL;                        /* current row of Stirling numbers */
    static size_t S_cap = 0;

    /* Initialisation on first call */
    if (computed_max == (idx_t)-1)
    {
        /* Start with capacity for n=0 */
        a_cap = 1; a_cache = alloc_seq_array(a_cap); a_cache[0] = (seq_t)1;
        S_cap = 1; S = alloc_seq_array(S_cap); S[0] = (seq_t)1;
        computed_max = 0;
    }

    /* Grow tables if needed up to n */
    ensure_capacity(n + 1, &a_cache, &a_cap);
    ensure_capacity(n + 1, &S, &S_cap);

    /* Compute rows up to n */
    for (idx_t m = computed_max + 1; m <= n; ++m)
    {
        /* Create S[m] by recurrence:   S[m][k] = k*S[m][k] + S[m][k-1]  (k from m to 1) */
        if (m >= (idx_t)S_cap)              /* already ensured by ensure_capacity */
            ;                              
        S[m] = 0;                           /* start with zero for k=m (will be overwritten) */
        for (idx_t k = m; k >= 1; --k)
            S[k] = k * S[k] + S[k-1];        /* reuse array in place */
        S[0] = 0;                            /* S(m,0) = 0 for m>0 */

        /* Evaluate the sum for a(m).  For m=0 we already know a(0)=1. */
        if (m == 0) continue;

        seq_t sum = 0;
        seq_t prod = 1;                      /* product fac[m+2]/fac[m-k+3] for current k */

        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t term = 3 * S[k] * prod;
            if (k & 1)
                sum -= term;
            else
                sum += term;

            /* Update 'prod' for next k+1: prod *= (m - k + 3) */
            prod *= (seq_t)(m - k + 3);
        }

        a_cache[m] = sum;
    }

    computed_max = (idx_t) (n > computed_max ? n : computed_max);
    return a_cache[n];
}
