/*
 * A380208  Expansion of e.g.f. exp( (1+3*x)^(1/3) - 1 ).
 * https://oeis.org/A380208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380208: Expansion of e.g.f. exp( (1+3*x)^(1/3) - 1 )
 *
 * Formula (OEIS notation):
 *   a(0) = 1
 *   a(n) = Σ_{k=1..n} (Product_{j=0..k-1} (-3*j+1)) * binomial(n-1, k-1) * a(n-k)
 *
 * The implementation memoises the required products and the sequence itself
 * so that calls may be made in any order.  It uses only integer arithmetic
 * and a simple dynamic array grow routine.
 */

#include <stdlib.h>     /* malloc, realloc, abort */
#include <stddef.h>     /* size_t */

/* Helper that ensures the array *arr has at least 'n+1' elements.
   *arr is resized by realloc; *cap is updated accordingly. */
static void grow_cache(idx_t n, seq_t **arr, size_t *cap)
{
    size_t needed = (size_t)(n + 1);
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* unrecoverable allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *a_cache = NULL;
    static size_t a_cap  = 0;
    static size_t a_size = 0;     /* number of valid entries in a_cache */

    /* Cached product P[k] = ∏_{j=0..k-1} (1-3*j)  (P[0]=1) */
    static seq_t *P_cache = NULL;
    static size_t P_cap   = 0;
    static size_t P_size  = 0;

    /* Ensure space for the requested index in both caches */
    grow_cache(n, &a_cache, &a_cap);
    grow_cache(n, &P_cache, &P_cap);

    /* Initialise base case if this is the first call */
    if (a_size == 0) {
        a_cache[0] = (seq_t)1;
        a_size = 1;
    }
    if (P_size == 0) {
        P_cache[0] = (seq_t)1;
        P_size = 1;
    }

    /* Compute the product array P up to index n if not already done */
    for (idx_t k = P_size; k <= n; ++k) {
        if (k == 0) {
            P_cache[0] = (seq_t)1;
        } else {
            seq_t term = (seq_t)1 - (seq_t)3 * (seq_t)(k - 1);
            P_cache[k] = P_cache[k - 1] * term;
        }
    }
    P_size = n + 1;

    /* Compute sequence values a[m] for m = a_size .. n */
    for (idx_t m = a_size; m <= n; ++m) {
        if (m == 0) {
            a_cache[0] = (seq_t)1;    /* already set, but keep for completeness */
            continue;
        }

        seq_t sum = (seq_t)0;
        /* bin = binomial(m-1, k-1) handled iteratively */
        seq_t bin = (seq_t)1;          /* for k = 1, binomial(m-1,0) = 1 */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = P_cache[k] * bin * a_cache[m - k];
            sum += term;

            if (k < m) {
                /* Update binomial for next k:
                   bin = C(m-1, k) = C(m-1, k-1) * (m-k) / k . */
                bin = bin * (seq_t)(m - k) / (seq_t)k;
            }
        }
        a_cache[m] = sum;
    }
    a_size = n + 1;

    return a_cache[n];
}
