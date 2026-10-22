/*
 * A363745  Number of integer partitions of n whose rounded-down mean is 2.
 * https://oeis.org/A363745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of integer partitions of n whose rounded‑down mean is 2.
   a(n) = Σ_{k=floor(n/3)+1}^{floor(n/2)} p_k(n)   where p_k(n) = number of
   partitions of n into exactly k parts. */

#include <stdlib.h>
#include <stdint.h>

/* external integer types */
typedef uint64_t idx_t;

/* --------------------------------------------------------------------- */
/* The function a(n) ---------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Work only for n ≥ 0. For n = 0, the answer is 0 by definition. */
    if (n == 0)
        return 0;

    /* Convert the size into a type suitable for array indices. */
    const size_t N = (size_t)n;             /* n itself is at most N */
    const size_t maxHalf = N / 2;           /* maximum number of parts needed */

    /* Allocate working arrays.  All arrays are of size N+1. */
    seq_t *prev = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *ans  = (seq_t*)malloc((N + 1) * sizeof(seq_t));

    /* If the allocation failed, return 0 (no value). */
    if (!prev || !curr || !ans) {
        free(prev); free(curr); free(ans);
        return 0;
    }

    /* Initialise arrays to zero. */
    for (size_t i = 0; i <= N; ++i) {
        prev[i] = 0;
        curr[i] = 0;
        ans[i]  = 0;
    }
    /* p(0,0) = 1, used for the recurrence. */
    prev[0] = 1;

    /* Main DP: iterate over the number of parts k. */
    for (size_t k = 1; k <= maxHalf; ++k) {
        /* Compute p_k(n) for all n = 0..N using the recurrence
           p_k(n) = p_{k-1}(n-1) + p_k(n-k).  */
        for (size_t nidx = 1; nidx <= N; ++nidx) {
            seq_t v1 = 0;
            seq_t v2 = 0;
            if (nidx >= 1)  v1 = prev[nidx - 1];   /* p_{k-1}(n-1) */
            if (nidx >= k)  v2 = curr[nidx - k];   /* p_k(n-k)    */
            curr[nidx] = v1 + v2;
        }

        /* Accumulate into ans[n] for all n where this k is in the
           required range floor(n/3)+1 ≤ k ≤ floor(n/2). */
        for (size_t nidx = 0; nidx <= N; ++nidx) {
            size_t low  = nidx / 3 + 1;  /* floor(n/3)+1 */
            size_t high = nidx / 2;      /* floor(n/2)   */
            if (k >= low && k <= high) {
                ans[nidx] = ans[nidx] + curr[nidx];
            }
        }

        /* Prepare for the next iteration: copy curr into prev. */
        for (size_t i = 0; i <= N; ++i) {
            prev[i] = curr[i];
        }
    }

    /* Retrieve the answer for the requested n. */
    seq_t result = ans[N];

    /* Free dynamically allocated memory. */
    free(prev);
    free(curr);
    free(ans);

    return result;
}
