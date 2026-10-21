/*
 * A283040  Number of nX6 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors, with the exception of exactly
 *          one element.
 * https://oeis.org/A283040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×6 binary arrays with no 1 having more than
 *            one * horizontal or vertical neighbour, except for exactly one
 *            element that is allowed * to have >1 neighbours
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h> /* for memset */

#define NUM_COLS 6
#define NUM_MASKS (1 << NUM_COLS)
#define TRANS_ENTRIES (NUM_MASKS * NUM_MASKS)

/* Precomputed transitions for all pairs of rows (a,b) and possible next row c.
 * Only triples (a,b,c) that do not produce more than one violating cell in row b
 * are stored.  transSize[idx] gives the number of valid continuations.
 */
static uint8_t transC[TRANS_ENTRIES][NUM_MASKS];
static uint8_t transV[TRANS_ENTRIES][NUM_MASKS]; /* 0 or 1 violation */
static uint8_t transSize[TRANS_ENTRIES] = {0};
static bool init_done = false;

/* Compute number of violating cells in middle row b when surrounded by
 * vertical neighbours a (up) and c (down).
 * Returns 0,1 or 2 (2 meaning >1 violation, so invalid).
 */
static inline int compute_violations(uint8_t a, uint8_t b, uint8_t c)
{
    int viol = 0;
    for (int j = 0; j < NUM_COLS; ++j)
    {
        if ((b >> j) & 1)
        {
            int cnt = 0;
            if (j > 0 && ((b >> (j - 1)) & 1)) ++cnt;
            if (j < NUM_COLS - 1 && ((b >> (j + 1)) & 1)) ++cnt;
            cnt += (a >> j) & 1;
            cnt += (c >> j) & 1;
            if (cnt > 1)
            {
                ++viol;
                if (viol > 1) return 2;
            }
        }
    }
    return viol;
}

/* One‑time precomputation of all valid triples (a,b,c). */
static void init_trans(void)
{
    if (init_done) return;
    for (uint8_t a = 0; a < NUM_MASKS; ++a)
    {
        for (uint8_t b = 0; b < NUM_MASKS; ++b)
        {
            uint16_t idx = a * NUM_MASKS + b;
            uint8_t sz = 0;
            for (uint8_t c = 0; c < NUM_MASKS; ++c)
            {
                int v = compute_violations(a, b, c);
                if (v <= 1)
                {
                    transC[idx][sz] = c;
                    transV[idx][sz] = (uint8_t)v;
                    ++sz;
                }
            }
            transSize[idx] = sz;
        }
    }
    init_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_trans();

    /* DP arrays: dp[a][b][used] – number of ways to reach state where
     * the last two rows are a (row i-1) and b (row i), with 'used' exception(s)
     * already applied (0 or 1). */
    static seq_t dp[NUM_MASKS][NUM_MASKS][2];
    static seq_t ndp[NUM_MASKS][NUM_MASKS][2];

    /* Zero DP arrays */
    for (int a = 0; a < NUM_MASKS; ++a)
        for (int b = 0; b < NUM_MASKS; ++b)
            for (int u = 0; u < 2; ++u)
                dp[a][b][u] = 0;

    /* Initial state: top dummy row a = 0, first real row b = any mask */
    for (int b = 0; b < NUM_MASKS; ++b)
        dp[0][b][0] = 1;

    /* Process remaining rows: we already have the first row, need to add n-1 more */
    for (idx_t step = 1; step < n; ++step)
    {
        /* Zero ndp */
        for (int a = 0; a < NUM_MASKS; ++a)
            for (int b = 0; b < NUM_MASKS; ++b)
                for (int u = 0; u < 2; ++u)
                    ndp[a][b][u] = 0;

        for (int a = 0; a < NUM_MASKS; ++a)
        {
            for (int b = 0; b < NUM_MASKS; ++b)
            {
                uint16_t idx = a * NUM_MASKS + b;
                uint8_t sz = transSize[idx];
                for (int u = 0; u < 2; ++u)
                {
                    seq_t ways = dp[a][b][u];
                    if (ways == 0) continue;
                    for (uint8_t t = 0; t < sz; ++t)
                    {
                        uint8_t c = transC[idx][t];
                        uint8_t v = transV[idx][t];
                        int nu = u + v;
                        if (nu <= 1)
                            ndp[b][c][nu] += ways;
                    }
                }
            }
        }

        /* Copy ndp to dp for next iteration */
        for (int a = 0; a < NUM_MASKS; ++a)
            for (int b = 0; b < NUM_MASKS; ++b)
                for (int u = 0; u < 2; ++u)
                    dp[a][b][u] = ndp[a][b][u];
    }

    /* Final step: apply bottom dummy row (all zeros) to the last real row */
    seq_t answer = 0;
    for (int a = 0; a < NUM_MASKS; ++a)
    {
        for (int b = 0; b < NUM_MASKS; ++b)
        {
            for (int u = 0; u < 2; ++u)
            {
                seq_t ways = dp[a][b][u];
                if (ways == 0) continue;
                int v = compute_violations(a, b, 0);
                if (v <= 1 && u + v == 1)
                    answer += ways;
            }
        }
    }
    return answer;
}
