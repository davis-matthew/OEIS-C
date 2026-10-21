/*
 * A269009  Number of nX6 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with exactly one adjacent
 *            pair (horizontal, diagonal or antidiagonal)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a dynamic‑programming scan over the rows.
   It works for any seq_t defined by the build system (primitive unsigned
   types up to 128 bits or arbitrary‑precision). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* OEIS offset is 1 */

    /* Pre‑compute horizontal adjacency within a row and
       diagonal adjacency between consecutive rows. */
    static bool built = false;
    static int  horiz[64];
    static int  cross[64][64];

    if (!built)
    {
        for (int p = 0; p < 64; ++p)
        {
            unsigned int m = (unsigned int)p;
            /* popcount of (m & (m >> 1)) counts pairs 11 inside the row */
            horiz[p] = __builtin_popcount(m & (m >> 1));
        }

        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
            {
                unsigned int ap = (unsigned int)a;
                unsigned int bp = (unsigned int)b;
                /* pairs on the down‑right and down‑left diagonals */
                unsigned int left  = bp & ((ap << 1) & 0x3Fu);
                unsigned int right = bp & (ap >> 1);
                cross[a][b] = __builtin_popcount(left) + __builtin_popcount(right);
            }

        built = true;
    }

    /* dp_prev[pattern][count] – number of ways to reach the current
       row pattern with 'count' adjacent 1‑pairs seen so far (count ∈ {0,1}) */
    seq_t dp_prev[64][2];
    seq_t dp_next[64][2];

    for (int p = 0; p < 64; ++p)
    {
        dp_prev[p][0] = (seq_t)0;
        dp_prev[p][1] = (seq_t)0;
    }

    /* first row: only horizontal adjacency contributes */
    for (int p = 0; p < 64; ++p)
    {
        int c = horiz[p];
        if (c <= 1) dp_prev[p][c] = (seq_t)1;
    }

    /* process subsequent rows */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (int p = 0; p < 64; ++p)
        {
            dp_next[p][0] = (seq_t)0;
            dp_next[p][1] = (seq_t)0;
        }

        for (int prev = 0; prev < 64; ++prev)
        {
            seq_t v0 = dp_prev[prev][0];
            if (v0)
            {
                for (int cur = 0; cur < 64; ++cur)
                {
                    int newc = horiz[cur] + cross[prev][cur];
                    if (newc <= 1) dp_next[cur][newc] += v0;
                }
            }

            seq_t v1 = dp_prev[prev][1];
            if (v1)
            {
                for (int cur = 0; cur < 64; ++cur)
                {
                    int newc = 1 + horiz[cur] + cross[prev][cur];
                    if (newc <= 1) dp_next[cur][newc] += v1;
                }
            }
        }

        /* copy next layer into previous for next iteration */
        for (int p = 0; p < 64; ++p)
        {
            dp_prev[p][0] = dp_next[p][0];
            dp_prev[p][1] = dp_next[p][1];
        }
    }

    /* sum all paths that resulted in exactly one adjacent pair */
    seq_t answer = (seq_t)0;
    for (int p = 0; p < 64; ++p) answer += dp_prev[p][1];
    return answer;
}
