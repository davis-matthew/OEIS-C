/*
 * A286011  a(1)=1, and for n>1, a(n) is the maximum number of iterations of
 *          sigma resulting in n, starting at some integer k; or 0 if n cannot
 *          be reached from any k.
 * https://oeis.org/A286011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum number of iterations of sigma that produce n
 *            from some integer k (t ≥ 1). a(1) is defined as 1. The algorithm
 *            builds the sequence up to the requested n by iterating over all
 *            k ≤ n, computing sigma(k) and propagating the maximum depth
 *            found so far
 * Range:     u8 n <= 4367, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case for the first element */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Static cache for the computed values up to the largest n
       requested so far.  This allows the function to be called
       in arbitrary order without recomputing from scratch. */
    static seq_t *dp = NULL;
    static idx_t maxComputed = 0;

    if (n > maxComputed) {
        /* Allocate space for dp[0..n] (dp[0] unused) */
        size_t newSize = (size_t)(n + 1);
        seq_t *dp_new = (seq_t *)calloc(newSize, sizeof(seq_t));
        if (!dp_new)
            return (seq_t)0;   /* allocation failed */

        dp_new[1] = (seq_t)1;  /* a(1) = 1 */

        /* Compute sigma values for 1..n */
        seq_t *sigma = (seq_t *)calloc(newSize, sizeof(seq_t));
        if (!sigma) {
            free(dp_new);
            return (seq_t)0;
        }

        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i)
                sigma[j] += (seq_t)i;
        }

        /* Propagate depths from each k to sigma(k) */
        for (idx_t k = 2; k <= n; ++k) {
            seq_t sig = sigma[k];
            if (sig > (seq_t)n)            /* skip if outside range */
                continue;
            idx_t s = (idx_t)sig;
            seq_t new_depth = dp_new[k] + (seq_t)1;
            if (new_depth > dp_new[s])
                dp_new[s] = new_depth;
        }

        free(sigma);
        free(dp);      /* discard old cache */
        dp = dp_new;
        maxComputed = n;
    }

    return dp[n];
}
