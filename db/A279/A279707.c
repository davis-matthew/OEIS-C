/*
 * A279707  Number of nX6 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A279707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A279707: number of n×6 0/1 arrays with no element equal to a strict
 * majority of its horizontal and antidiagonal neighbours, and with new
 * values introduced in order 0 sequentially upwards. 
 * The new‑value condition is enforced by requiring the top‑left cell to be 0,
 * eliminating the symmetry that caused double counting in earlier attempts. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the function that will be implemented below. */
seq_t a(idx_t n);

/* Helper: bit of a row at column i (0 ≤ i < 6). */
static inline int bit(int row, int i) { return (row >> i) & 1; }

/* Check if a single row satisfies the rule when it is the only row
 * (no vertical or antidiagonal neighbours). */
static inline bool single_ok(int row)
{
    for (int i = 0; i < 6; ++i) {
        int sum = 0, cnt = 0;
        if (i > 0)   { sum += bit(row, i-1); ++cnt; }
        if (i < 5)   { sum += bit(row, i+1); ++cnt; }
        if (sum*2 > cnt && bit(row,i) == 1) return false;
        if (sum*2 < cnt && bit(row,i) == 0) return false;
    }
    return true;
}

/* Check if the first row satisfies the rule with the second row only. */
static inline bool start_ok(int row, int next)
{
    for (int i = 0; i < 6; ++i) {
        int sum = 0, cnt = 0;
        if (i > 0)   { sum += bit(row, i-1); ++cnt; }
        if (i < 5)   { sum += bit(row, i+1); ++cnt; }
        if (i > 0)   { sum += bit(next, i-1); ++cnt; }   /* down‑left */
        if (sum*2 > cnt && bit(row,i) == 1) return false;
        if (sum*2 < cnt && bit(row,i) == 0) return false;
    }
    return true;
}

/* Check if the last row satisfies the rule with the previous row only. */
static inline bool end_ok(int row, int prev)
{
    for (int i = 0; i < 6; ++i) {
        int sum = 0, cnt = 0;
        if (i > 0)   { sum += bit(row, i-1); ++cnt; }
        if (i < 5)   { sum += bit(row, i+1); ++cnt; }
        if (i < 5)   { sum += bit(prev, i+1); ++cnt; }   /* up‑right */
        if (sum*2 > cnt && bit(row,i) == 1) return false;
        if (sum*2 < cnt && bit(row,i) == 0) return false;
    }
    return true;
}

/* Check if the middle row of a triple satisfies the rule with both neighbours. */
static inline bool middle_ok(int prev, int cur, int next)
{
    for (int i = 0; i < 6; ++i) {
        int sum = 0, cnt = 0;
        if (i > 0)   { sum += bit(cur, i-1); ++cnt; }
        if (i < 5)   { sum += bit(cur, i+1); ++cnt; }
        if (i < 5)   { sum += bit(prev, i+1); ++cnt; }   /* up‑right */
        if (i > 0)   { sum += bit(next, i-1); ++cnt; }   /* down‑left */
        if (sum*2 > cnt && bit(cur,i) == 1) return false;
        if (sum*2 < cnt && bit(cur,i) == 0) return false;
    }
    return true;
}

/* Tables that are computed once.  They are indexed by row patterns (0..63). */
static bool init_done = false;
static bool g_start_ok[64][64];
static bool g_end_ok[64][64];
static uint64_t g_next_mask[64][64];   /* bits 0..63 set if transition is allowed */

/* Initialise the static tables. */
static void init_tables(void)
{
    if (init_done) return;
    for (int p = 0; p < 64; ++p)
        for (int q = 0; q < 64; ++q) {
            g_start_ok[p][q] = start_ok(p, q);
            g_end_ok[p][q]   = end_ok(p, q);
        }

    for (int pp = 0; pp < 64; ++pp)
        for (int p = 0; p < 64; ++p) {
            uint64_t mask = 0;
            for (int n = 0; n < 64; ++n)
                if (middle_ok(pp, p, n))
                    mask |= 1ULL << n;
            g_next_mask[pp][p] = mask;
        }
    init_done = true;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;      /* undefined for n<1, but safe guard */

    /* ---------- n == 1 ---------------------------------- */
    if (n == 1) {
        seq_t cnt = 0;
        for (int r = 0; r < 64; ++r) {
            if ((r & 1) == 0 && single_ok(r))   /* top-left cell must be 0 */
                cnt += (seq_t)1;
        }
        return cnt;
    }

    /* ---------- n == 2 ---------------------------------- */
    if (n == 2) {
        seq_t cnt = 0;
        for (int r1 = 0; r1 < 64; ++r1) {
            if ((r1 & 1) == 0) {          /* top‑left must be 0 */
                for (int r2 = 0; r2 < 64; ++r2)
                    if (g_start_ok[r1][r2] && g_end_ok[r2][r1])
                        cnt += (seq_t)1;
            }
        }
        return cnt;
    }

    /* ---------- n >= 3 ---------------------------------- */
    static seq_t dp[64][64];
    static seq_t ndp[64][64];

    /* Zero dp matrix */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;

    /* Initialise with first two rows, respecting the start condition. */
    for (int r1 = 0; r1 < 64; ++r1) {
        if ((r1 & 1) == 0) {          /* top-left must be 0 */
            for (int r2 = 0; r2 < 64; ++r2) {
                if (g_start_ok[r1][r2])
                    dp[r1][r2] = 1;
            }
        }
    }

    /* Iterate over remaining rows. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                ndp[i][j] = 0;

        for (int pp = 0; pp < 64; ++pp) {
            for (int p = 0; p < 64; ++p) {
                seq_t w = dp[pp][p];
                if (w == 0) continue;
                uint64_t mask = g_next_mask[pp][p];
                while (mask) {
                    unsigned int nrow = __builtin_ctzll(mask);
                    ndp[p][nrow] += w;
                    mask &= mask - 1;
                }
            }
        }

        /* Copy ndp back into dp for the next iteration. */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over all ending states that satisfy the end condition. */
    seq_t ans = 0;
    for (int prev = 0; prev < 64; ++prev)
        for (int cur = 0; cur < 64; ++cur)
            if (g_end_ok[cur][prev])
                ans += dp[prev][cur];
    return ans;
}
