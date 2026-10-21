/*
 * A274194  Row sums of the array A274193, defined by g(n,k) = 1 for n >= 0;
 *          g(n,k) = 0 if k > n; g(n,k) = g(n-1,k-1) + g(n-1,3k) for n > 0, k
 *          > 1.
 * https://oeis.org/A274194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^n g(n,k) where g is defined by g(0,0)=1,
 *            g(n,0)=1, and for n>0 and 1≤k≤n: g(n,k)=g(n-1,k-1)+g(n-1,3k).
 *            a(0)=1. This implementation computes a(n) iteratively, memoising
 *            from the last computed row
 * Range:     u8 n <= 18, u16 n <= 39, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of rows and results */
    static seq_t *prev = NULL;      /* previous row (size max_n+1) */
    static seq_t *curr = NULL;      /* current row being computed */
    static seq_t *a_cache = NULL;   /* cached a values */
    static idx_t  max_n = 0;        /* highest n computed so far */
    static size_t cap = 0;          /* allocated capacity */

    /* initialise on first call */
    if (!prev) {
        cap = 1;
        prev = (seq_t *)malloc(cap * sizeof(seq_t));
        curr = (seq_t *)malloc(cap * sizeof(seq_t));
        a_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!prev || !curr || !a_cache) return 0;  /* memory failure */
        prev[0] = (seq_t)1;
        a_cache[0] = (seq_t)1;
        max_n = 0;
    }

    /* enlarge allocation if needed */
    if ((idx_t)(cap) <= n) {
        size_t new_cap = (size_t)(n + 1);
        prev = (seq_t *)realloc(prev, new_cap * sizeof(seq_t));
        curr = (seq_t *)realloc(curr, new_cap * sizeof(seq_t));
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!prev || !curr || !a_cache) return 0;  /* memory failure */
        cap = new_cap;
    }

    /* compute rows up to n if not already done */
    while (max_n < n) {
        idx_t new_n = max_n + 1;   /* row to compute */
        idx_t old_n = max_n;       /* previous row length minus one */

        /* first element of the new row is always 1 */
        curr[0] = (seq_t)1;
        seq_t sum = (seq_t)1;      /* sum for a(new_n) */

        for (idx_t k = 1; k <= new_n; ++k) {
            seq_t val = prev[k - 1];          /* g(new_n, k-1) from previous row */
            idx_t idx3 = 3 * k;               /* index for g(new_n, 3k) */
            if (idx3 <= old_n)                /* check bounds of previous row */
                val += prev[idx3];
            curr[k] = val;
            sum += val;
        }

        a_cache[new_n] = sum;          /* cache a(new_n) */
        /* swap pointers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
        max_n = new_n;
    }

    return a_cache[n];
}
