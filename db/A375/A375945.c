/*
 * A375945  Expansion of e.g.f. 1 / (1 + 2 * log(1 - x))^(3/2).
 * https://oeis.org/A375945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2k+1)!! * unsigned Stirling1(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where (2k+1)!! is the odd double factorial and unsigned Stirling1(n,k) counts
   permutations of n with exactly k cycles.  The unsigned Stirling numbers are
   computed with the recurrence: S(n,0)=0 (n>0), S(n,k)=S(n-1,k-1)+(n-1)*S(n-1,k),
   and S(0,0)=1.  The implementation works for any seq_t type provided by the
   build, using only the integer operators required by the problem statement.  */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Special case for n=0 */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* Convert index to a size_t for array sizing */
    size_t N = (size_t)n;

    /* Allocate arrays to hold two successive rows of unsigned Stirling numbers */
    seq_t *prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base row: S(0,0) = 1 */
    prev[0] = (seq_t)1;

    /* Pre‑compute odd double factorials (2k+1)!! up to k = N */
    seq_t *df = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!df) {
        free(prev);
        free(curr);
        return (seq_t)0;
    }
    df[0] = (seq_t)1;
    for (size_t k = 1; k <= N; ++k) {
        df[k] = df[k - 1] * ((seq_t)(2 * k + 1));
    }

    /* Iterate over n = 1 .. N to build the required row of Stirling numbers */
    for (size_t i = 1; i <= N; ++i) {
        curr[0] = (seq_t)0;
        for (size_t k = 1; k <= i; ++k) {
            curr[k] = prev[k - 1] + prev[k] * ((seq_t)(i - 1));
        }

        /* If this is the target row, compute the sum */
        if (i == N) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k <= N; ++k) {
                sum += df[k] * curr[k];
            }

            /* Clean up */
            free(prev);
            free(curr);
            free(df);

            return sum;
        }

        /* Prepare for next iteration: swap prev and curr, and zero out curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
        /* curr will be overwritten in the next iteration, no need to zero it */
    }

    /* Should not reach here */
    free(prev);
    free(curr);
    free(df);
    return (seq_t)0;
}
