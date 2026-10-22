/*
 * A347711  Number of compositions (ordered partitions) of n into at most 4
 *          squares.
 * https://oeis.org/A347711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..4} A337165(n,k)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for dynamic programming.  Arrays are zero‑filled. */
    size_t N = (size_t)n;
    seq_t *prev;
    seq_t *curr;
    seq_t *ans;
#ifdef __cplusplus
    prev = new seq_t[N + 1]();
    curr = new seq_t[N + 1]();
    ans  = new seq_t[N + 1]();
#else
    prev = (seq_t*)calloc(N + 1, sizeof(seq_t));
    curr = (seq_t*)calloc(N + 1, sizeof(seq_t));
    ans  = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif

    /* Precompute the list of square numbers up to n. */
    idx_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n)
        ++sqrt_n;
    size_t sq_count = (size_t)sqrt_n;
    idx_t *sqs = NULL;
    if (sq_count > 0) {
#ifdef __cplusplus
        sqs = new idx_t[sq_count];
#else
        sqs = (idx_t*)malloc(sq_count * sizeof(idx_t));
#endif
        for (size_t i = 0; i < sq_count; ++i) {
            idx_t v = (idx_t)(i + 1);
            sqs[i] = v * v;
        }
    }

    /* Base case: empty composition for n = 0. */
    prev[0] = 1;
    ans[0]  = 1;

    /* Compute contributions for compositions of 1 to 4 squares. */
    for (idx_t k = 1; k <= 4; ++k) {
        /* Zero the current DP array (curr).  calloc does this in C;
           new[] with value initialization does it in C++. */
#ifdef __cplusplus
        std::fill(curr, curr + N + 1, (seq_t)0);
#else
        for (size_t i = 0; i <= N; ++i) curr[i] = 0;
#endif

        for (idx_t i = 0; i <= n; ++i) {
            seq_t ways = prev[i];
            if (!ways) continue;
            idx_t remaining = n - i;
            for (size_t m = 0; m < sq_count; ++m) {
                idx_t s = sqs[m];
                if (s > remaining) break;
                size_t pos = (size_t)i + (size_t)s;   /* guaranteed: pos <= N */
                curr[pos] += ways;
            }
        }

        /* Add current stage to the overall answer. */
        for (idx_t i = 0; i <= n; ++i)
            ans[i] += curr[i];

        /* Swap prev and curr for the next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Clean up. */
    seq_t result = ans[(size_t)n];
#ifdef __cplusplus
    delete[] prev;
    delete[] curr;
    delete[] ans;
    if (sqs) delete[] sqs;
#else
    free(prev);
    free(curr);
    free(ans);
    if (sqs) free(sqs);
#endif

    return result;
}
