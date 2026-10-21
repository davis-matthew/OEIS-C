/*
 * A282991  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors.
 * https://oeis.org/A282991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n×3 0..1 arrays with no 1 having more than one orthogonal
   neighbor (horizontal or vertical). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Checks that a single row of length 3 is valid when it has
   predecessor row 'prev' and successor row 'next'.  Each element
   that is 1 must have at most one neighbouring 1 among its
   horizontal and vertical contacts. */
static inline bool row_valid(unsigned curr, unsigned prev, unsigned next)
{
    for (unsigned c = 0; c < 3; ++c) {
        if ((curr >> c) & 1U) {
            unsigned cnt = 0;
            /* left neighbour */
            if (c > 0 && ((curr >> (c - 1)) & 1U)) ++cnt;
            /* right neighbour */
            if (c < 2 && ((curr >> (c + 1)) & 1U)) ++cnt;
            /* upper neighbour */
            if ((prev >> c) & 1U) ++cnt;
            /* lower neighbour */
            if ((next >> c) & 1U) ++cnt;
            if (cnt > 1) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* no defined value below offset */

    /* Cache of previously computed terms: cache[1] .. cache[max_n] */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* largest n for which cache[n] is valid */

    /* DP tables for state transition */
    static seq_t dp[8][8];
    static seq_t ndp[8][8];

    /* Ensure cache can hold up to n. */
    if (cache_len < n) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1000;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if (cache_len >= n) return cache[n];

    /* Compute a(1) if not yet done. */
    if (cache_len == 0) {
        seq_t ans1 = 0;
        for (unsigned m = 0; m < 8; ++m)
            if (row_valid(m, 0, 0)) ++ans1;
        cache[1] = ans1;
        cache_len = 1;
    }

    /* Compute a(2) if not yet done. */
    if (cache_len == 1 && n >= 2) {
        /* Initialise dp for the two‑row system. */
        for (unsigned r0 = 0; r0 < 8; ++r0)
            for (unsigned r1 = 0; r1 < 8; ++r1)
                dp[r0][r1] = row_valid(r0, 0, r1) ? 1 : 0;

        seq_t ans2 = 0;
        for (unsigned r0 = 0; r0 < 8; ++r0)
            for (unsigned r1 = 0; r1 < 8; ++r1)
                if (dp[r0][r1] && row_valid(r1, r0, 0))
                    ans2 += dp[r0][r1];
        cache[2] = ans2;
        cache_len = 2;
    }

    /* Build the dp for larger n incrementally. */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        if (cur == 2) continue;   /* already handled */

        /* Transition: ndp[currow][next] += dp[prev][currow] if next row is valid. */
        for (unsigned i = 0; i < 8; ++i)
            for (unsigned j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for (unsigned prev = 0; prev < 8; ++prev) {
            for (unsigned currow = 0; currow < 8; ++currow) {
                seq_t ways = dp[prev][currow];
                if (ways == 0) continue;
                for (unsigned nxt = 0; nxt < 8; ++nxt)
                    if (row_valid(currow, prev, nxt))
                        ndp[currow][nxt] += ways;
            }
        }

        /* Copy ndp back to dp. */
        for (unsigned i = 0; i < 8; ++i)
            for (unsigned j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];

        /* Compute a(cur): count end states where last row has no lower neighbour. */
        seq_t anscur = 0;
        for (unsigned prev = 0; prev < 8; ++prev)
            for (unsigned currow = 0; currow < 8; ++currow)
                if (dp[prev][currow] && row_valid(currow, prev, 0))
                    anscur += dp[prev][currow];

        cache[cur] = anscur;
    }

    cache_len = n;
    return cache[n];
}
