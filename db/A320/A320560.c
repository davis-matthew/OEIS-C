/*
 * A320560  Number of set partitions of [n] such that for each block b the
 *          smallest integer interval containing b has at most ten elements
 *          and for at least one block c the smallest integer interval
 *          containing c has exactly ten elements.
 * https://oeis.org/A320560
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276726(n) – A276725(n)
 * Range:     u8 none, u16 n <= 10, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 499 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276726(n) = number of set partitions of [n] where every block's
 * smallest‑containing integer interval has length ≤ 10.
 * A276725(n) = same with length ≤ 9.
 * Hence a(n) counts partitions with at least one block of exact
 * interval length 10.  The function works for all n ≥ 10.
 *
 * The implementation uses a state DP over a bitmask of currently
 * "open" blocks.  For a limit k the mask has k‑1 bits, one for each
 * possible distance from the current element to the start of an
 * open block.  At step i (element i+1) each open block can either
 * be closed or extended; a new block may be started.
 * This is a standard Fubini‑like DP with O(n·2^{k−1}) operations.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t count_partitions(idx_t n, unsigned int k)
{
    /* The maximum number of states needed is 2^(k-1) ≤ 512.
       Allocate the biggest array once and reuse it for k=9 and k=10.  */
    enum { MAX_STATES = 512 };
    static seq_t dp[MAX_STATES], dp_next[MAX_STATES];

    idx_t maskSize = ((idx_t)1) << (k - 1);
    idx_t maskMask = maskSize - 1;

    /* Initialise dp for this k */
    for (idx_t m = 0; m < maskSize; ++m) {
        dp[m] = 0;
    }
    dp[0] = 1;

    for (idx_t i = 0; i < n; ++i) {
        /* Zero dp_next */
        for (idx_t m = 0; m < maskSize; ++m) {
            dp_next[m] = 0;
        }

        for (idx_t mask = 0; mask < maskSize; ++mask) {
            seq_t val = dp[mask];
            if (!val)
                continue;

            unsigned int open = __builtin_popcount((unsigned int)mask);
            idx_t shiftMask = (mask << 1) & maskMask;

            /* attach current element to any existing open block */
            if (open)
                dp_next[shiftMask] += val * (seq_t)open;
            /* start a new block containing current element */
            dp_next[shiftMask | 1] += val;
        }

        /* swap dp and dp_next for next iteration */
        for (idx_t m = 0; m < maskSize; ++m) {
            dp[m] = dp_next[m];
        }
    }

    /* Sum all states: all blocks must be closed at the end. */
    seq_t result = 0;
    for (idx_t m = 0; m < maskSize; ++m) {
        result += dp[m];
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 10)      /* offset is 10; behaviour for smaller n is irrelevant */
        return 0;

    seq_t full = count_partitions(n, 10);   /* blocks ≤10 */
    seq_t short_len = count_partitions(n, 9); /* blocks ≤9 */
    return full - short_len;
}
