/*
 * A268993  Number of nX6 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes a(n) for OEIS A268993
   Number of n×6 binary arrays with at most one pair of 1s that are
   horizontally or antidiagonally adjacent. */

/* The DP below counts adjacency pairs: horizontal pairs within a row
   and anti‑diagonal pairs between consecutive rows.  The anti‑diagonal
   adjacency is taken to be cells (i‑1,j+1) and (i,j) for each 0≤j<6. */

seq_t a(idx_t n)
{
    /* Pre‑compute horizontal pair counts for each row mask. */
    static unsigned char h_pairs[64];
    static unsigned char ad_pairs[64][64];
    static bool initialized = false;

    if (!initialized)
    {
        for (int m = 0; m < 64; ++m)
        {
            unsigned char cnt = 0;
            for (int j = 0; j < 5; ++j)
                if (((m >> j) & 1u) && ((m >> (j + 1)) & 1u))
                    ++cnt;
            h_pairs[m] = cnt;
        }

        /* Anti‑diagonal adjacency between previous (p) and current (c) rows.
           Only the diagonal (p,j+1) ↔ (c,j) is counted.  This corresponds
           to the pair (i-1,j+1) and (i,j). */
        for (int p = 0; p < 64; ++p)
        {
            for (int c = 0; c < 64; ++c)
            {
                unsigned char cnt = 0;
                for (int j = 0; j < 6; ++j)
                {
                    if ((c >> j) & 1u)
                        if (j < 5 && ((p >> (j + 1)) & 1u))
                            ++cnt;          /* prev[j+1] – curr[j] */
                }
                ad_pairs[p][c] = cnt;
            }
        }
        initialized = true;
    }

    /* DP tables: dp_cur[mask][cnt] where cnt is 0 or 1. */
    static seq_t dp_cur[64][2];
    static seq_t dp_next[64][2];

    /* Initialise first row. */
    for (int m = 0; m < 64; ++m)
    {
        dp_cur[m][0] = 0;
        dp_cur[m][1] = 0;
        unsigned char h = h_pairs[m];
        if (h <= 1)
            dp_cur[m][h] = 1;
    }

    if (n == 1)
    {
        seq_t total = 0;
        for (int m = 0; m < 64; ++m)
        {
            total += dp_cur[m][0];
            total += dp_cur[m][1];
        }
        return total;
    }

    /* Process rows 2 .. n. */
    for (idx_t row = 2; row <= n; ++row)
    {
        /* Zero next DP array. */
        for (int m = 0; m < 64; ++m)
        {
            dp_next[m][0] = 0;
            dp_next[m][1] = 0;
        }

        /* Transition from previous row. */
        for (int pm = 0; pm < 64; ++pm)
        {
            for (int pcnt = 0; pcnt < 2; ++pcnt)
            {
                seq_t val = dp_cur[pm][pcnt];
                if (val == 0) continue;

                for (int cm = 0; cm < 64; ++cm)
                {
                    unsigned short total = pcnt + h_pairs[cm] + ad_pairs[pm][cm];
                    if (total <= 1)
                        dp_next[cm][total] += val;
                }
            }
        }

        /* Copy next to current for next iteration. */
        for (int m = 0; m < 64; ++m)
        {
            dp_cur[m][0] = dp_next[m][0];
            dp_cur[m][1] = dp_next[m][1];
        }
    }

    /* Sum over all masks and counts. */
    seq_t result = 0;
    for (int m = 0; m < 64; ++m)
    {
        result += dp_cur[m][0];
        result += dp_cur[m][1];
    }
    return result;
}
