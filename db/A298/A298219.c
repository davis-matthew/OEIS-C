/*
 * A298219  Number of nX6 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n): number of n×6 binary arrays with upper left element 0,
 * such that for every cell the number of king‑move adjacent cells
 * having the same value is in {0,1,2,4,5}.
 * Computation by dynamic programming over pairs of consecutive rows.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* helper to extract bit c (0..5) of pattern p */
static inline int bit(uint64_t p, int c)
{
    return (p >> c) & 1;
}

/* true iff the count of equal neighbors for cell col c (0..5)
 * in row cur with optional up row prev and down row next is
 * one of {0,1,2,4,5}. prev/next == (uint64_t)-1 means the row does not exist. */
static bool count_neighbors_ok(uint64_t cur, uint64_t prev, uint64_t next)
{
    for (int c = 0; c < 6; ++c) {
        int val = bit(cur, c);
        int cnt = 0;
        /* same row neighbors */
        if (c > 0 && bit(cur, c - 1) == val) ++cnt;
        if (c < 5 && bit(cur, c + 1) == val) ++cnt;
        /* upper neighbors */
        if (prev != (uint64_t)-1) {
            if (c > 0 && bit(prev, c - 1) == val) ++cnt;
            if (bit(prev, c) == val) ++cnt;
            if (c < 5 && bit(prev, c + 1) == val) ++cnt;
        }
        /* lower neighbors */
        if (next != (uint64_t)-1) {
            if (c > 0 && bit(next, c - 1) == val) ++cnt;
            if (bit(next, c) == val) ++cnt;
            if (c < 5 && bit(next, c + 1) == val) ++cnt;
        }
        /* disallow counts 3,6,7,8 */
        if (cnt == 3 || cnt == 6 || cnt == 7 || cnt == 8)
            return false;
    }
    return true;
}

/* valid for first row (no upper row) */
static bool valid_top(uint64_t cur, uint64_t next)
{
    return count_neighbors_ok(cur, (uint64_t)-1, next);
}

/* valid for last row (no lower row) */
static bool valid_last(uint64_t prev, uint64_t cur)
{
    return count_neighbors_ok(cur, prev, (uint64_t)-1);
}

/* valid for a single row with no neighbors (n==1) */
static bool valid_single(uint64_t cur)
{
    return count_neighbors_ok(cur, (uint64_t)-1, (uint64_t)-1);
}

/* global tables */
static bool initialized = false;
static uint64_t trans_masks[64][64];  /* for each (prev,cur) the set of allowed next rows */

/* initialize tables once */
static void init_tables(void)
{
    if (initialized) return;
    for (uint64_t prev = 0; prev < 64; ++prev) {
        for (uint64_t cur = 0; cur < 64; ++cur) {
            uint64_t mask = 0;
            for (uint64_t next = 0; next < 64; ++next) {
                if (count_neighbors_ok(cur, prev, next))
                    mask |= (1ULL << next);
            }
            trans_masks[prev][cur] = mask;
        }
    }
    initialized = true;
}

/* main DP routine */
seq_t a(idx_t n)
{
    init_tables();

    /* allocate DP arrays (static to preserve across calls) */
    static seq_t *dp   = NULL;
    static seq_t *tmp  = NULL;
    const size_t SZ = 64 * 64;
    if (!dp) {
        dp  = (seq_t *)malloc(SZ * sizeof(seq_t));
        tmp = (seq_t *)malloc(SZ * sizeof(seq_t));
        memset(dp, 0, SZ * sizeof(seq_t));
        memset(tmp, 0, SZ * sizeof(seq_t));
    }

    /* n==1 special case */
    if (n == 1) {
        seq_t ans = 0;
        for (uint64_t row = 0; row < 64; ++row) {
            if (bit(row, 0) != 0) continue;           /* upper left must be 0 */
            if (valid_single(row))
                ans += 1;
        }
        return ans;
    }

    /* clear dp */
    memset(dp, 0, SZ * sizeof(seq_t));

    /* initialisation: first two rows */
    for (uint64_t r0 = 0; r0 < 64; ++r0) {
        if (bit(r0, 0) != 0) continue;               /* upper left zero */
        for (uint64_t r1 = 0; r1 < 64; ++r1) {
            if (!valid_top(r0, r1)) continue;
            size_t idx = r0 * 64 + r1;
            dp[idx] = 1;
        }
    }

    /* DP over remaining rows (row indices 2 .. n-1) */
    for (idx_t row = 1; row <= n - 2; ++row) {
        /* zero tmp */
        memset(tmp, 0, SZ * sizeof(seq_t));

        for (uint64_t prev = 0; prev < 64; ++prev) {
            for (uint64_t cur = 0; cur < 64; ++cur) {
                seq_t val = dp[prev * 64 + cur];
                if (!val) continue;
                uint64_t mask = trans_masks[prev][cur];
                while (mask) {
                    uint64_t next = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    size_t idx2 = cur * 64 + next;
                    tmp[idx2] += val;
                }
            }
        }
        /* swap arrays */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
    }

    /* final row (n-1) check */
    seq_t ans = 0;
    for (uint64_t prev = 0; prev < 64; ++prev) {
        for (uint64_t cur = 0; cur < 64; ++cur) {
            seq_t val = dp[prev * 64 + cur];
            if (!val) continue;
            if (valid_last(prev, cur))
                ans += val;
        }
    }
    return ans;
}
