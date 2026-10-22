/*
 * A375904  E.g.f. satisfies A(x) = (1 - log(1 - x * A(x)^(1/2)))^2.
 * https://oeis.org/A375904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * sum_{k=1..n} |Stirling1(n,k)| * ∏_{u=n-k+3}^{n+1} u
 *            (Here Stirling1 denotes unsigned Stirling numbers of the first
 *            kind. a(0) = 1. The recurrence for Stirling numbers is S(n,k) =
 *            S(n-1,k-1) + (n-1)*S(n-1,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /* static cache for already computed values   */
    static seq_t *cache = NULL;     /* cache[0..max_n]    */
    static idx_t cache_len = 0;     /* current size of cache  */

    /* static arrays for current and previous row of unsigned Stirling
       numbers of the first kind:  prev[k] = S(i-1,k)  ,  curr[k] = S(i,k)   */
    static seq_t *prev = NULL;
    static seq_t *curr = NULL;
    static idx_t row_cap = 0;       /* capacity of prev/curr arrays   */

    /* --- initialise on first call ------------------------------------- */
    if (cache_len == 0) {
        cache = seq_alloc(1);            /* cache[0] */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        cache_len = 1;

        prev = seq_alloc(1);             /* first row: S(0,0)=1 */
        prev[0] = (seq_t)1;
        curr = seq_alloc(1);             /* will be overwritten before use */
        curr[0] = (seq_t)0;
        row_cap = 1;
    }

    /* --- extend cache if necessary ------------------------------------- */
    if (n >= cache_len) {
        idx_t old_cache_len = cache_len;
        idx_t new_cache_len = n + 1;
        seq_t *new_cache = seq_alloc(new_cache_len);
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        for (idx_t i = cache_len; i < new_cache_len; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_len = new_cache_len;
        /* compute only for the newly added indices afterwards */
        cache_len = old_cache_len;        /* will be replaced after row loop */
    }

    /* --- extend row arrays if necessary -------------------------------- */
    if (row_cap < n + 1) {
        idx_t new_row_cap = n + 1;
        seq_t *new_prev = seq_alloc(new_row_cap);
        for (idx_t i = 0; i < row_cap; ++i)
            new_prev[i] = prev[i];
        /* No need to initialise the new part of prev */

        seq_t *new_curr = seq_alloc(new_row_cap);
        /* new_curr will be filled anew in the loop */

        prev = new_prev;
        curr = new_curr;
        row_cap = new_row_cap;
    }

    /* Compute additional values up to n if necessary */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* --- compute row S(i,k) --- */
        curr[0] = (seq_t)0;                     /* S(i,0)=0 for i>0 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t part1 = prev[k - 1];          /* S(i-1,k-1)   */
            seq_t part2 = (k <= i - 1) ?
                            ((seq_t)(i - 1)) * prev[k]  /* (i-1)*S(i-1,k) */
                          : (seq_t)0;
            curr[k] = part1 + part2;
        }

        /* --- compute a(i) from the row --- */
        seq_t sum  = (seq_t)0;
        seq_t prod = (seq_t)1;                  /* product_{u=n-k+3}^{n+1} u */
        for (idx_t k = 1; k <= i; ++k) {
            if (k >= 2)                         /* add factor (i-k+3) */
                prod *= (seq_t)(i - k + 3);
            sum += curr[k] * prod;
        }
        cache[i] = sum * (seq_t)2;              /* multiply by 2 */

        /* --- prepare for next iteration --- */
        seq_t *tmp  = prev;
        prev = curr;
        curr = tmp;
    }

    /* After computing, cache_len must reflect the highest computed index. */
    cache_len = n + 1;

    return cache[n];
}
