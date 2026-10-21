/*
 * A286929  Number of partitions of n such that each part is no more than 3
 *          more than the sum of all smaller parts.
 * https://oeis.org/A286929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A286929.
 * Sequence definition: a(n) = number of partitions of n such that for every part
 *   p the inequality p ≤ (sum of all smaller parts) + 3 holds.
 *
 * This implementation uses dynamic programming.  For each part size `part`
 * we add it to all partial sums `s` that satisfy the condition
 *   part ≤ (s - part) + 3   ⇔   s ≥ 2*part - 3 .
 * The algorithm runs in O(n²) time and uses O(n) space.
 *
 * The function has no static state; it allocates its own array for each call
 * and frees it before returning the result.  It works for any integer type
 * `seq_t` (unsigned of any size or arbitrary-precision class) and any
 * index type `idx_t` (uint64_t or equivalent).
 */

#include <stdlib.h>   /* malloc, calloc, free */
#include <stdint.h>   /* int definitions */

seq_t a(idx_t n)
{
    /* Allocate and zero the DP array. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure -> return 0 */

    dp[0] = (seq_t)1;           /* base case : empty partition of 0 */

    for (idx_t part = 1; part <= n; ++part) {
        /* Compute threshold: part ≤ (s - part) + 3  ⇔  s ≥ 2*part - 3. */
        int thr = 2 * (int)part - 3;          /* safe for part ≤ 10000   */
        idx_t start = part;                   /* s must be at least part */
        if (thr > (int)part) start = (idx_t)thr;

        for (idx_t s = start; s <= n; ++s) {
            /* s ≥ part ensures s-part ≥ 0; dp[s-part] is already computed. */
            dp[s] += dp[s - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
