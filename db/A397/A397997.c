/*
 * A397997  E.g.f. A(x) satisfies A((exp(3*x) - 1)/3) = 4*A(x) - 3*x.
 * https://oeis.org/A397997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Σ_{k=1..n} 3^{n-k} * Stirling2(n+1,k) * a(k)
 *            (Stirling2(0,0)=1, Stirling2(n,0)=0 for n>0) Implements an
 *            O(n^2) memoised recurrence suitable for up to n≈10000
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_TYPE_DEFINED
/* The build supplies seq_t and idx_t.  Do not undefine. */
#endif

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Handle the trivial case directly. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Static mutable state for memoisation. */
    static seq_t *a_arr = NULL;     /* a_arr[i] = a(i) */
    static seq_t *pow3  = NULL;     /* pow3[i] = 3^i  (i >= 0) */
    static seq_t *S     = NULL;     /* S[k] holds Stirling2(current_n, k) */
    static idx_t cap = 0;           /* allocated size for the above arrays   */
    static idx_t pow3_len = 0;      /* number of valid entries in pow3      */
    static idx_t cur_n  = 0;        /* largest n for which a_arr[n] is set   */

    /* Initialise the static containers on the first call. */
    if (!a_arr) {
        /* Allocate initial capacity. */
        cap = 4;                                       /* 0..3 */
        a_arr = (seq_t *)calloc(cap, sizeof(seq_t));
        pow3  = (seq_t *)calloc(cap, sizeof(seq_t));
        S     = (seq_t *)calloc(cap, sizeof(seq_t));

        a_arr[1] = (seq_t)1;      /* a(1) = 1 */
        pow3[0]  = (seq_t)1;      /* 3^0   */
        pow3[1]  = (seq_t)3;      /* 3^1   */
        pow3_len = 2;

        S[1] = (seq_t)1;          /* Stirling(1,1) = 1 */
        cur_n = 1;
    }

    /* If the requested term is already computed, return it. */
    if (n <= cur_n)
        return a_arr[n];

    /* Ensure that the arrays can hold indices up to n+1. */
    if (n + 1 >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n + 1) new_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        pow3  = (seq_t *)realloc(pow3,  new_cap * sizeof(seq_t));
        S     = (seq_t *)realloc(S,     new_cap * sizeof(seq_t));
        for (idx_t i = cap; i < new_cap; ++i) {
            a_arr[i] = 0;
            pow3[i]  = 0;
            S[i]     = 0;
        }
        cap = new_cap;
    }

    /* Main loop: grow the memoised table up to n. */
    for (idx_t m = cur_n; m < n; ++m) {
        /* Make sure the powers of 3 are computed up to exponent m. */
        while (pow3_len <= m) {
            pow3[pow3_len] = pow3[pow3_len - 1] * (seq_t)3;
            ++pow3_len;
        }

        /* Update Stirling numbers in-place for n = m+1. */
        /* We use reverse order so that S[k-1] from the previous step
           isn't overwritten before it is used.                */
        for (idx_t k = m + 1; k >= 1; --k) {
            if (k == 1)
                S[1] = (seq_t)1;
            else
                S[k] = (seq_t)k * S[k] + S[k - 1];
        }

        /* Compute a(m+1) using the recurrence. */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            /* term = 3^{m-k} * S[k] * a(k) */
            seq_t term = pow3[m - k];
            term = term * S[k];
            term = term * a_arr[k];
            sum += term;
        }
        a_arr[m + 1] = sum;
    }

    cur_n = n;
    return a_arr[n];
}
