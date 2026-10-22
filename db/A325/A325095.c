/*
 * A325095  Number of subsets of {1...n} with no binary carries.
 * https://oeis.org/A325095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) counts subsets of {1..n} with pairwise disjoint binary
 *            1‑bits. Computation by dynamic programming over the bit mask of
 *            already used bits
 * Range:     u8 n <= 31, u16 n <= 271, u32 n <= 9974, u64 n <= 9974,
 *            u128 n <= 9974, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* number of bits needed to represent n */
    idx_t tmp = n;
    int bits = 0;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }
    size_t size = 1ull << bits;

    /* static DP buffer */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    if (dp_cap < size) {
        seq_t *newdp = (seq_t *)calloc(size, sizeof(seq_t));
        free(dp);
        dp = newdp;
        dp_cap = size;
    } else {
        memset(dp, 0, size * sizeof(seq_t));
    }

    dp[0] = (seq_t)1;

    for (idx_t num = 1; num <= n; ++num) {
        /* bit mask of the current number */
        size_t mask = 0;
        idx_t v = num;
        int pos = 0;
        while (v) {
            if (v & 1ull)
                mask |= 1ull << pos;
            v >>= 1;
            ++pos;
        }

        /* update DP in descending order to avoid reusing the same number */
        for (size_t m = size; m > 0; --m) {
            size_t idx = m - 1;
            if ((idx & mask) == 0) {
                dp[idx | mask] += dp[idx];
            }
        }
    }

    seq_t total = 0;
    for (size_t m = 0; m < size; ++m)
        total += dp[m];
    return total;
}
