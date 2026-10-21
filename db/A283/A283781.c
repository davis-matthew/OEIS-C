/*
 * A283781  Number of n X 5 0..1 arrays with no 1 equal to more than two of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts the number of n×5 binary arrays that have
   exactly two cells violating the rule that a ‘1’ must have no more
   than two neighbors that are also ‘1’.  The neighbors considered are
   the horizontal, diagonal and antidiagonal neighbors in the 3×3
   surrounding window.  The algorithm performs a dynamic programme
   over successive rows.  States are triples (prev, curr, v) where
   prev and curr are 5‑bit row patterns and v∈{0,1,2} is the number of
   violations seen so far.  For each new row we add it to the state,
   update the violation counter using a pre‑computed
   transition table trans[prev][curr][next], and keep only states
   with v≤2.  After processing all rows and a zero row below the
   last one, we sum the states with exactly two total violations. */

#include <stdbool.h>
#include <stdint.h>

/* compile‑time constants */
enum { MASKS = 32, VMAX = 3 };

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* transition table: trans[p][c][n] is the number of violations
       caused by row `c` when its upper neighbour is `p` and lower
       neighbour is `n`. */
    static uint8_t trans[MASKS][MASKS][MASKS];
    static bool trans_inited = false;

    if (!trans_inited) {
        for (int p = 0; p < MASKS; ++p) {
            for (int c = 0; c < MASKS; ++c) {
                for (int nx = 0; nx < MASKS; ++nx) {
                    uint8_t viol = 0;
                    for (int j = 0; j < 5; ++j) {
                        if ((c >> j) & 1U) {           /* centre cell is 1 */
                            int cnt = 0;
                            /* horizontal neighbours */
                            if (j > 0)   cnt += (c >> (j-1)) & 1U;
                            if (j < 4)   cnt += (c >> (j+1)) & 1U;
                            /* diagonal from above */
                            if (j > 0)   cnt += (p >> (j-1)) & 1U;
                            if (j < 4)   cnt += (p >> (j+1)) & 1U;
                            /* antidiagonal from below */
                            if (j > 0)   cnt += (nx >> (j-1)) & 1U;
                            if (j < 4)   cnt += (nx >> (j+1)) & 1U;
                            if (cnt > 2) ++viol;
                        }
                    }
                    trans[p][c][nx] = viol;
                }
            }
        }
        trans_inited = true;
    }

    /* dynamic programming table: dp[parity][prev][curr][v] */
    static seq_t dp[2][MASKS][MASKS][VMAX];
    /* zero the table for every call */
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < MASKS; ++b)
            for (int c = 0; c < MASKS; ++c)
                for (int v = 0; v < VMAX; ++v)
                    dp[a][b][c][v] = 0;

    int cur = 0, nxt = 1;

    /* initial row – there is a dummy zero row above */
    for (int curr = 0; curr < MASKS; ++curr)
        dp[cur][0][curr][0] = 1;

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next state */
        for (int a = 0; a < MASKS; ++a)
            for (int b = 0; b < MASKS; ++b)
                for (int v = 0; v < VMAX; ++v)
                    dp[nxt][a][b][v] = 0;

        for (int prev = 0; prev < MASKS; ++prev) {
            for (int curr = 0; curr < MASKS; ++curr) {
                for (int v = 0; v < VMAX; ++v) {
                    const seq_t val = dp[cur][prev][curr][v];
                    if (!val) continue;
                    for (int nx = 0; nx < MASKS; ++nx) {
                        int v2 = v + trans[prev][curr][nx];
                        if (v2 < VMAX)
                            dp[nxt][curr][nx][v2] += val;
                    }
                }
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* final transition: row below the last one is zero */
    for (int a = 0; a < MASKS; ++a)
        for (int b = 0; b < MASKS; ++b)
            for (int v = 0; v < VMAX; ++v)
                dp[nxt][a][b][v] = 0;

    for (int prev = 0; prev < MASKS; ++prev) {
        for (int curr = 0; curr < MASKS; ++curr) {
            for (int v = 0; v < VMAX; ++v) {
                const seq_t val = dp[cur][prev][curr][v];
                if (!val) continue;
                int v2 = v + trans[prev][curr][0];
                if (v2 < VMAX)
                    dp[nxt][curr][0][v2] += val;
            }
        }
    }

    /* sum states with exactly two violations */
    seq_t result = 0;
    for (int prev = 0; prev < MASKS; ++prev)
        result += dp[nxt][prev][0][2];

    return result;
}
