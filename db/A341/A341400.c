/*
 * A341400  Number of nonnegative solutions to (x_1)^2 + (x_2)^2 + ... +
 *          (x_5)^2 <= n.
 * https://oeis.org/A341400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer 5‑tuples (x1,…,x5) with
 *            x1²+…+x5² ≤ n. The code uses dynamic programming over dimensions
 *            and sums the counts for all sums up to n
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Length of DP arrays */
    size_t len = (size_t)n + 1;

    /* Allocate DP arrays */
    seq_t *dp_prev = (seq_t *)alloc_seq(len);
    seq_t *dp_cur  = (seq_t *)alloc_seq(len);

    /* Precompute squares up to n */
    size_t maxk = 0;
    while ((uint64_t)(maxk + 1) * (uint64_t)(maxk + 1) <= n) ++maxk;
    size_t sq_len = maxk + 1;
    uint64_t *sq = (uint64_t *)malloc(sq_len * sizeof(uint64_t));
    for (size_t i = 0; i < sq_len; ++i)
        sq[i] = (uint64_t)i * (uint64_t)i;

    /* Initialise dp_prev for exactly one square */
    for (size_t s = 0; s < len; ++s)
        dp_prev[s] = (seq_t)0;
    for (size_t k = 0; k < sq_len; ++k)
        dp_prev[(size_t)sq[k]] = (seq_t)1;
    dp_prev[0] = (seq_t)1;  /* 0 can always be represented as 0² */

    /* DP over dimensions 2 through 5 */
    for (size_t d = 2; d <= 5; ++d) {
        /* Zero dp_cur */
        for (size_t s = 0; s < len; ++s)
            dp_cur[s] = (seq_t)0;

        /* Build counts for current dimension */
        for (size_t s = 0; s < len; ++s) {
            seq_t acc = (seq_t)0;
            for (size_t i = 0; i < sq_len && sq[i] <= s; ++i) {
                acc += dp_prev[s - (size_t)sq[i]];
            }
            dp_cur[s] = acc;
        }

        /* Swap dp_prev and dp_cur for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
    }

    /* Sum counts for all sums ≤ n */
    seq_t total = (seq_t)0;
    for (size_t s = 0; s < len; ++s)
        total += dp_prev[s];

    /* Free allocated memory */
    free_seq(dp_prev);
    free_seq(dp_cur);
    free(sq);

    return total;
}
