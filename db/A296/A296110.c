/*
 * A296110  Number of n X 3 0..1 arrays with each 1 adjacent to 3 or 4
 *          king-move neighboring 1's.
 * https://oeis.org/A296110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+3*a(n-2)+15*a(n-3)+2*a(n-4)−18*a(n-5)−36*a(n-6)−21*a(n-7)−25*a(n-8)+3*a(n-9)+11*a(n-10)+6*a(n-11).
 *            Computed by dynamic programming over the 3×n binary matrices
 *            that satisfy the “1 has 3 or 4 king‑neighbouring 1’s” condition
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The types idx_t and seq_t are supplied by the build system (no redefinition). */

/* Directions for king moves in a 3×3 neighbourhood. */
static const int di[8] = {-1,-1,-1, 0,0, 1,1,1};
static const int dj[8] = {-1, 0, 1,-1,1,-1,0,1};

/* Count king‑neighbouring 1’s of a cell in column 'col' when the rows
   previous, current and next are given as 3‑bit integers. */
static inline int neighbour_count(unsigned int prev, unsigned int cur,
                                  unsigned int next, int col)
{
    int cnt = 0;
    for (int k = 0; k < 8; ++k) {
        int nc = col + dj[k];
        if (nc < 0 || nc > 2) continue;
        if (di[k] == -1) {
            if (prev & (1u << nc)) ++cnt;
        } else if (di[k] == 0) {
            if (cur  & (1u << nc)) ++cnt;
        } else { /* di[k] == 1 */
            if (next & (1u << nc)) ++cnt;
        }
    }
    return cnt;
}

/* Precomputed validity table: valid[prev][cur][next] is true iff
   every 1 in the middle row 'cur' has 3 or 4 king‑neighbouring 1’s
   when surrounded by rows 'prev' and 'next'. */
static bool valid[8][8][8];
static bool validity_computed = false;

static void compute_validity(void)
{
    for (int prev = 0; prev < 8; ++prev) {
        for (int cur = 0; cur < 8; ++cur) {
            for (int next = 0; next < 8; ++next) {
                bool ok = true;
                for (int col = 0; col < 3; ++col) {
                    if (cur & (1u << col)) {
                        int v = neighbour_count(prev, cur, next, col);
                        if (v != 3 && v != 4) { ok = false; break; }
                    }
                }
                valid[prev][cur][next] = ok;
            }
        }
    }
    validity_computed = true;
}

/* Main sequence function.  Uses a 64‑state DP that runs in O(n) time. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but be defensive */
    if (!validity_computed) compute_validity();

    static seq_t dp[8][8], ndp[8][8];
    /* clear dp */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;

    /* first row: previous row is all 0 */
    for (int cur = 0; cur < 8; ++cur)
        dp[0][cur] = 1;

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = 0;
        for (int prev = 0; prev < 8; ++prev) {
            for (int cur = 0; cur < 8; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                for (int next = 0; next < 8; ++next) {
                    if (valid[prev][cur][next])
                        ndp[cur][next] += ways;
                }
            }
        }
        /* copy ndp to dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum states where the final row cur is valid with below row 0 */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int cur = 0; cur < 8; ++cur) {
            if (dp[prev][cur] && valid[prev][cur][0])
                result += dp[prev][cur];
        }
    }
    return result;
}
