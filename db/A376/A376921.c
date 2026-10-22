/*
 * A376921  Number T(n,k) of binary words of length n avoiding distance (i+1)
 *          between "1" digits if the i-th bit is set in the binary
 *          representation of k; triangle T(n,k), n>=0,
 *          0<=k<=ceiling(2^(n-1))-1, read by rows.
 * https://oeis.org/A376921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) counts binary words of length r avoiding forbidden
 *            distances * according to the key k derived from the index n,
 *            i.e. the entry T(r,k) of * the triangle described in A376921.
 *            The function uses a dynamic programming * method that is
 *            O(r·2^(r‑1)), where r ≈ log2(n). For any n ≥ 0 it returns * t...
 * Range:     u8 n <= 127, u16 n <= 9772, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* handle row 0 -> only one element, value 1 */
    if (n == 0) return (seq_t)1;

    /* r = floor_log2(n) + 1, i.e. the row number of the element at index n */
    uint64_t r = (uint64_t)(64 - __builtin_clzll(n));   /* n > 0 */

    /* start index of this row: 2^(r‑1) */
    uint64_t start = (uint64_t)1 << (r - 1);

    /* index k within the row (0 ≤ k < 2^(r‑1)) */
    uint64_t k = n - start;

    /* DP over positions with a bitmask of the last (r‑1) bits.
       Number of states = 2^(r‑1).  For r = 1 we still use 1 state. */
    size_t states = (r >= 1) ? ((size_t)1 << (r - 1)) : 1;
    static seq_t dp[1 << 13];   /* 8192 is enough for r ≤ 14 */
    static seq_t ndp[1 << 13];
    size_t mask_limit = (r > 1) ? (((size_t)1 << (r - 1)) - 1) : 0U;

    /* initialise dp */
    memset(dp, 0, states * sizeof(seq_t));
    dp[0] = (seq_t)1;

    /* iterate over each position */
    for (uint64_t pos = 0; pos < r; ++pos) {
        /* clear next dp array */
        memset(ndp, 0, states * sizeof(seq_t));

        for (size_t mask = 0; mask < states; ++mask) {
            seq_t cur = dp[mask];
            if (!cur) continue;

            /* place a 0 */
            size_t nm = (mask << 1) & mask_limit;
            ndp[nm] += cur;

            /* place a 1, allowed iff no forbidden distance conflicts */
            if ((mask & k) == 0) {
                size_t nm1 = ((mask << 1) | 1) & mask_limit;
                ndp[nm1] += cur;
            }
        }
        /* copy back to dp for next iteration */
        memcpy(dp, ndp, states * sizeof(seq_t));
    }

    /* sum all dp[mask] to get total count */
    seq_t total = 0;
    for (size_t i = 0; i < states; ++i)
        total += dp[i];

    return total;
}
