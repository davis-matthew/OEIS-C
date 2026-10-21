/*
 * A281613  Expansion of Sum_{i>=1} x^(i^3)/(1 - x^(i^3)) / Product_{j>=1} (1
 *          - x^(j^3)).
 * https://oeis.org/A281613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into cubes
 * Range:     u8 n <= 50, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, calloc, free */

/* Computes the n-th term of OEIS A281613.  idx_t is an unsigned integer
   type (e.g., uint64_t), seq_t is an unsigned integer type that may be
   arbitrary precision or a standard sized integer. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n == 0. */
    if (n == 0)
        return 0;

    /* Allocate DP arrays; use calloc to zero-initialize. */
    size_t size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *ps = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp || !ps) {
        free(dp);
        free(ps);
        return 0;
    }

    dp[0] = 1; /* empty partition */
    /* ps[0] is already zero from calloc */

    /* Iterate over cube coin types 1^3, 2^3, … up to floor(cuberoot(n)). */
    for (idx_t i = 1; ; ++i) {
        idx_t cube = i * i * i;
        if (cube > n)
            break;
        for (idx_t k = cube; k <= n; ++k) {
            /* New partitions that use at least one cube of size 'cube'. */
            dp[k]          += dp[k - cube];
            /* Sum of parts: previous parts plus the added cube. */
            ps[k]          += ps[k - cube] + dp[k - cube];
        }
    }

    seq_t result = ps[n];
    free(dp);
    free(ps);
    return result;
}
