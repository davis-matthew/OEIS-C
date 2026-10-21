/*
 * A289501  Number of enriched p-trees of weight n.
 * https://oeis.org/A289501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + p(n))/2, where p(0)=1 and for n≥1 p(n) is the
 *            weighted partition sum defined by p(n) = Σ_{i=1}^n a(i) * p(n-i)
 *            (with proper multiplicities) The following implements a
 *            recomputation of all terms up to the largest argument requested
 *            using dynamic programming over partition parts. The im...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;         /* array of a(n) values   */
static seq_t *dp = NULL;        /* DP array for weighted partitions */
static idx_t  computed_max = 0; /* maximum n that has been computed */
static size_t capacity     = 0; /* current allocated size   */

seq_t a(idx_t n)
{
    /* Ensure the base case a(0)=1 and dp[0]=1 exists. */
    if (A == NULL) {
        capacity = 1;
        A  = (seq_t *)malloc(capacity * sizeof(seq_t));
        dp = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (A == NULL || dp == NULL) return (seq_t)0;   /* allocation failure */
        A[0] = 1;
        dp[0] = 1;
        computed_max = 0;
    }

    if (n == 0) {
        return A[0];
    }

    /* If we have already computed up to n, just return the cached value. */
    if (n <= computed_max) {
        return A[n];
    }

    /* Allocate enough space for the new maximum. */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *newA  = (seq_t *)realloc(A,  new_cap * sizeof(seq_t));
        seq_t *newdp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (newA == NULL || newdp == NULL) return (seq_t)0; /* allocation failure */
        A  = newA;
        dp = newdp;
        capacity = new_cap;
    }

    /* Re‑compute everything from scratch up to n. */
    /* Reset dp to zero and set dp[0] = 1. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;
    A[0] = 1;          /* base value (a(0)=1) */

    for (idx_t i = 1; i <= n; ++i) {
        /* dp[i] currently equals p(i) using parts < i. */
        A[i] = ((seq_t)1) + dp[i];

        /* Update dp for all totals that can use part i (unlimited copies). */
        for (idx_t t = i; t <= n; ++t) {
            dp[t] += dp[t - i] * A[i];
        }
    }

    computed_max = n;
    return A[n];
}
