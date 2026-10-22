/*
 * A317275  a(1) = 1; a(n) = Sum_{k=1..n-1} |Stirling1(n-1,k)|*a(k)*a(n-k).
 * https://oeis.org/A317275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{n-1} |Stirling1(n-1,k)| * a(k) * a(n-k)
 *            Initial value: a(1) = 1 (Recurrence uses absolute Stirling
 *            numbers of the first kind.)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached data across calls */
    static idx_t  max_computed = 0;   /* largest n already computed */
    static size_t cap          = 0;   /* allocated array capacity */
    static seq_t *a_cache      = NULL;/* a[1..max_computed]          */
    static seq_t *prev        = NULL;/* Stirling row for n-1        */
    static seq_t *curr        = NULL;/* Temporary row for n         */

    if (n < 1) return 0; /* safety; OEIS offset is 1 */

    /* Extend arrays if needed */
    if (cap <= (size_t)n) {
        size_t old_cap = cap;
        size_t new_cap = (size_t)n + 1;           /* indices 0..n */
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        prev    = (seq_t *)realloc(prev,    new_cap * sizeof(seq_t));
        curr    = (seq_t *)realloc(curr,    new_cap * sizeof(seq_t));
        for (size_t i = old_cap; i < new_cap; ++i) {
            a_cache[i] = 0;
            prev[i]    = 0;
            curr[i]    = 0;
        }
        cap = new_cap;
    }

    /* Base initialization for n = 1 */
    if (max_computed == 0) {
        a_cache[1] = (seq_t)1; /* a(1) = 1 */
        prev[1]    = (seq_t)1; /* |Stirling1(1,1)| = 1 */
        max_computed = 1;
    }

    /* Compute up to the requested n */
    while (max_computed < n) {
        idx_t m = max_computed + 1;          /* next index to compute */
        seq_t sum = 0;

        /* a(m) = Σ_{k=1}^{m-1} prev[k] * a_cache[k] * a_cache[m-k] */
        for (idx_t k = 1; k < m; ++k) {
            seq_t prod = prev[k];
            prod *= a_cache[k];
            prod *= a_cache[m - k];
            sum += prod;
        }
        a_cache[m] = sum;

        /* Build next Stirling row (absolute values) for n = m */
        curr[1] = (seq_t)(m - 1) * prev[1]; /* |s(m,1)| = (m-1)*|s(m-1,1)| */
        for (idx_t k = 2; k < m; ++k) {
            curr[k] = prev[k - 1] + (seq_t)(m - 1) * prev[k];
        }
        curr[m] = prev[m - 1]; /* |s(m,m)| = 1 = |s(m-1,m-1)| */

        /* Swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;

        ++max_computed;
    }

    return a_cache[n];
}
