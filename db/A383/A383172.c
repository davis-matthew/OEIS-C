/*
 * A383172  Expansion of e.g.f. -log(1 + log(1 - 2*x)/2)^3 / 6.
 * https://oeis.org/A383172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=3..n} 2^(n-k) * |Stirling1(n,k) * Stirling1(k,3)|
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static tables for memoisation of Stirling tables, powers of 2
       and intermediate values a(n).  They are lazily grown. */
    static size_t capacity = 0;
    static idx_t max_computed = 0;
    static seq_t *prev = NULL;  /* Stirling numbers from row (i-1) */
    static seq_t *curr = NULL;  /* temporary storage for current row */
    static seq_t *pow2 = NULL;  /* pow2[i] = 2^i  */
    static seq_t *a3   = NULL;  /* a3[i] = Stirling1(i,3)  */
    static seq_t *res  = NULL;  /* final sequence values */

    /* initialise on first invocation */
    if (capacity == 0) {
        capacity = 4;                         /* start with indices 0..3 */
        prev = (seq_t*)calloc(capacity, sizeof(seq_t));
        curr = (seq_t*)calloc(capacity, sizeof(seq_t));
        pow2 = (seq_t*)calloc(capacity, sizeof(seq_t));
        a3   = (seq_t*)calloc(capacity, sizeof(seq_t));
        res  = (seq_t*)calloc(capacity, sizeof(seq_t));
        if (!prev || !curr || !pow2 || !a3 || !res) abort();

        pow2[0] = (seq_t)1;
        a3[0]   = (seq_t)0;

        res[0] = (seq_t)0;
        res[1] = (seq_t)0;
        res[2] = (seq_t)0;

        /* row 0: Stirling(0,0) = 1, others 0 */
        prev[0] = (seq_t)1;
        max_computed = 0;
    }

    /* return memoised value if already computed */
    if (n <= max_computed)
        return res[n];

    /* grow the tables if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t newcap = capacity;
        while ((size_t)n + 1 > newcap) newcap <<= 1;

        seq_t *tmp;

        tmp = (seq_t*)realloc(prev, newcap * sizeof(seq_t));
        if (!tmp) abort(); prev = tmp;
        tmp = (seq_t*)realloc(curr, newcap * sizeof(seq_t));
        if (!tmp) abort(); curr = tmp;
        tmp = (seq_t*)realloc(pow2, newcap * sizeof(seq_t));
        if (!tmp) abort(); pow2 = tmp;
        tmp = (seq_t*)realloc(a3, newcap * sizeof(seq_t));
        if (!tmp) abort(); a3 = tmp;
        tmp = (seq_t*)realloc(res, newcap * sizeof(seq_t));
        if (!tmp) abort(); res = tmp;

        for (size_t i = capacity; i < newcap; ++i) {
            prev[i] = (seq_t)0;
            curr[i] = (seq_t)0;
            pow2[i] = (seq_t)0;
            a3[i]   = (seq_t)0;
            res[i]  = (seq_t)0;
        }
        capacity = newcap;
    }

    /* build rows incrementally up to the requested n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            curr[k] = prev[k-1] + (seq_t)(i - 1) * prev[k];
        }

        a3[i] = (i >= 3 ? curr[3] : (seq_t)0);

        pow2[i] = pow2[i - 1] * (seq_t)2;

        seq_t sum = (seq_t)0;
        for (idx_t k = 3; k <= i; ++k) {
            seq_t term = curr[k] * a3[k];
            term *= pow2[i - k];
            sum += term;
        }
        res[i] = sum;

        /* prepare for next iteration: swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    max_computed = n;
    return res[n];
}
