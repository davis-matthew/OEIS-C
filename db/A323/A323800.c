/*
 * A323800  Number of permutations p of [n] such that max_{j=1..n} |p(j)-j| =
 *          4.
 * https://oeis.org/A323800
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = count_perm(n,4) – count_perm(n,3) where count_perm(n,k)
 *            counts permutations of [n] with max_{j} |p(j)-j| ≤ k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The DP keeps a sliding window of size 2k+1 and uses a bitmask
   to describe which positions in the current window are already
   occupied.  The leftmost position that falls out of the window
   must be matched if it is still free.  */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t count_perm(idx_t n, int k)
{
    if (n == 0) return 1;

    const size_t W   = 2u * (size_t)k + 1u;   /* window width */
    const size_t SZ  = 1u << W;               /* number of masks   */

    seq_t *dp  = mem_new(seq_t, SZ);
    seq_t *ndp = mem_new(seq_t, SZ);
    if (!dp || !ndp) return 0;   /* out of memory – unlikely */

    /* initial mask: positions outside [1,n] are marked as used */
    size_t initMask = 0;
    for (size_t t = 0; t < W; ++t) {
        idx_t pos = (idx_t)1 - (idx_t)k + (idx_t)t;
        if (pos < 1 || pos > n) initMask |= (1ULL << t);
    }
    dp[initMask] = 1;

    for (idx_t j = 1; j <= n; ++j) {
        /* reset next dp table */
        mem_set(ndp, 0, SZ * sizeof(seq_t));

        /* new position that will enter the window after this step */
        idx_t newPos = j + 1 + (idx_t)k;
        int   newBit = (newPos > n) ? 1 : 0;

        for (size_t mask = 0; mask < SZ; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;

            /* if leftmost position is free, it must be matched now */
            if ((mask & 1ULL) == 0ULL) {
                size_t shifted = (mask | 1ULL) >> 1;
                if (newBit) shifted |= (1ULL << (W - 1));
                ndp[shifted] += ways;
            } else {
                /* choose any free position inside the window */
                for (size_t t = 0; t < W; ++t) {
                    if ((mask & (1ULL << t)) == 0ULL) {
                        idx_t pos = (idx_t)j - (idx_t)k + (idx_t)t;
                        if (pos < 1 || pos > n) continue;
                        size_t shifted = (mask | (1ULL << t)) >> 1;
                        if (newBit) shifted |= (1ULL << (W - 1));
                        ndp[shifted] += ways;
                    }
                }
            }
        }

        /* swap tables for next iteration */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* sum of ways for all final states */
    seq_t total = 0;
    for (size_t mask = 0; mask < SZ; ++mask)
        total += dp[mask];

    mem_free(dp);
    mem_free(ndp);
    return total;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t up4 = count_perm(n, 4);
    seq_t up3 = count_perm(n, 3);
    return up4 - up3;
}
