/*
 * A277364  Number of ways to partition a set of n elements into at most n/2
 *          disjoint subsets.
 * https://oeis.org/A277364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} Stirling2(n,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of values for all n already computed */
    static seq_t *a_cache = NULL;          /* a_cache[i] = a(i)   */
    static seq_t *S      = NULL;          /* current row of Stirling numbers S(n,k), 0 <= k <= n */
    static idx_t cached_n = 0;            /* last n for which a_cache and S are up to date */
    static size_t a_cap = 0;              /* allocated size of a_cache   */
    static size_t S_cap = 0;              /* allocated size of S          */

    /* --- initialise on first call --------------------------------------- */
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;           /* a(0) = 1 */

        S_cap = 1;
        S = (seq_t *)malloc(S_cap * sizeof(seq_t));
        S[0] = (seq_t)1;                 /* S(0,0) = 1   */
        cached_n = 0;
    }

    /* If the required n is already known, return it directly */
    if (n <= cached_n) {
        return a_cache[n];
    }

    /* Ensure we have enough space in the cache arraya */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap;
        while ((size_t)(n + 1) > new_cap) {
            new_cap *= 2;          /* double until big enough */
        }
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* --- extend the computation up to n --------------------------------- */
    for (idx_t cur = cached_n + 1; cur <= n; ++cur) {
        /* Length of old and new rows of Stirling numbers */
        size_t old_len = cached_n + 1;    /* old row S[0..cached_n]    */
        size_t new_len = cur + 1;         /* new row S[0..cur]         */

        /* Allocate a fresh array for the new row */
        seq_t *newS = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!newS) {
            /* In a real project you might exit or signal an error */
            return (seq_t)0;
        }

        /* Compute the new row using the recurrence
           S(n,k) = k * S(n-1,k) + S(n-1,k-1) */
        for (size_t k = 0; k < new_len; ++k) {
            seq_t term1 = 0;
            seq_t term2 = 0;

            if (k <= old_len - 1) {
                term1 = S[k] * (seq_t)k;   /* k * S(n-1,k)   */
            }
            if (k >= 1 && (k-1) <= old_len - 1) {
                term2 = S[k-1];            /* S(n-1,k-1)     */
            }
            newS[k] = term1 + term2;
        }

        /* Sum the terms up to floor(cur/2) to get a(cur) */
        idx_t half = cur / 2;
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= half; ++k) {
            sum += newS[k];
        }
        a_cache[cur] = sum;

        /* Prepare for next iteration */
        free(S);
        S = newS;
        cached_n = cur;
    }

    /* Return the requested value */
    return a_cache[n];
}
