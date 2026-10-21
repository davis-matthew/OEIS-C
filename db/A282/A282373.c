/*
 * A282373  Number of nX4 0..1 arrays with no 1 equal to more than four of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence is computed by dynamic programming over rows of a
 *            4‑column 0..1 array. For each row we keep the last two row
 *            patterns, and the number of cells that have more than four
 *            1‑neighbors so far. Each 1 must have at most four king‑move
 *            neighbours, except exactly two cells in the whole array...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* precompute, for any three consecutive rows (up, mid, down), how many
   1’s in the middle row have more than four 1‑neighbors. */
static uint8_t v_table[16][16][16];
static bool v_init = false;

static void init_v(void)
{
    if (v_init) return;
    for (idx_t up = 0; up < 16; ++up)
    {
        for (idx_t mid = 0; mid < 16; ++mid)
        {
            for (idx_t down = 0; down < 16; ++down)
            {
                uint8_t violations = 0;
                for (int c = 0; c < 4; ++c)
                {
                    if (!(mid & (1u << c))) continue;
                    int neigh = 0;
                    for (int dr = -1; dr <= 1; ++dr)
                    {
                        idx_t row_pat = (dr == -1) ? up : (dr == 0) ? mid : down;
                        for (int dc = -1; dc <= 1; ++dc)
                        {
                            if (dr == 0 && dc == 0) continue;
                            int col = c + dc;
                            if (col < 0 || col >= 4) continue;
                            if (row_pat & (1u << col)) ++neigh;
                        }
                    }
                    if (neigh > 4) ++violations;
                }
                v_table[up][mid][down] = violations;
            }
        }
    }
    v_init = true;
}

/* Dynamic programming to compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1; though n>=1 in the problem   */

    /* DP tables: dp[prevprev][prev][viol]  . viol = 0..2 */
    static seq_t dp[16][16][3];
    static seq_t newdp[16][16][3];

    /* clear tables */
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t j = 0; j < 16; ++j)
            for (idx_t k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    init_v();

    /* first row: choose any pattern, no violations yet */
    for (idx_t p = 0; p < 16; ++p)
        dp[0][p][0] = 1;

    /* add rows 2 .. n */
    for (idx_t row = 1; row < n; ++row)
    {
        /* zero newdp */
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t j = 0; j < 16; ++j)
                for (idx_t k = 0; k < 3; ++k)
                    newdp[i][j][k] = 0;

        /* transition */
        for (idx_t up = 0; up < 16; ++up)
        {
            for (idx_t prev = 0; prev < 16; ++prev)
            {
                for (idx_t viol = 0; viol < 3; ++viol)
                {
                    seq_t cnt = dp[up][prev][viol];
                    if (cnt == 0) continue;
                    for (idx_t cur = 0; cur < 16; ++cur)
                    {
                        idx_t new_viol = viol + v_table[up][prev][cur];
                        if (new_viol > 2) continue;
                        newdp[prev][cur][new_viol] += cnt;
                    }
                }
            }
        }

        /* copy newdp into dp for next iteration */
        for (idx_t i = 0; i < 16; ++i)
            for (idx_t j = 0; j < 16; ++j)
                for (idx_t k = 0; k < 3; ++k)
                    dp[i][j][k] = newdp[i][j][k];
    }

    /* evaluate the last row (prev) with a below row of zeros */
    seq_t result = 0;
    for (idx_t up = 0; up < 16; ++up)
    {
        for (idx_t prev = 0; prev < 16; ++prev)
        {
            for (idx_t viol = 0; viol < 3; ++viol)
            {
                seq_t cnt = dp[up][prev][viol];
                if (cnt == 0) continue;
                idx_t final_viol = viol + v_table[up][prev][0]; /* below is 0 */
                if (final_viol == 2) result += cnt;
            }
        }
    }
    return result;
}
