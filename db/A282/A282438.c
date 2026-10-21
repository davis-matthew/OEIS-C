/*
 * A282438  Number of nX5 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors, with the exception of exactly one
 *          element.
 * https://oeis.org/A282438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of n×5 binary arrays in which exactly one 1 is allowed to have
 *  more than three 1‑neighbours by the king move (horizontal, vertical, diagonal).
 *  The values are computed by dynamic programming over the rows.
 */

#include <stdint.h>
#include <stdbool.h>

static int8_t bad_val[32][32][32] = {0};
static bool precomputed = false;

static void init_bad_val(void)
{
    if (precomputed) return;
    for (int above = 0; above < 32; ++above) {
        for (int cur   = 0; cur < 32; ++cur) {
            for (int nxt  = 0; nxt < 32; ++nxt) {
                int nbad = 0;
                for (int j = 0; j < 5; ++j) {
                    if ((cur >> j) & 1U) {
                        int neigh = 0;
                        /* above row */
                        if (j > 0) neigh += (above >> (j - 1)) & 1U;
                        neigh += (above >> j) & 1U;
                        if (j < 4) neigh += (above >> (j + 1)) & 1U;
                        /* same row */
                        if (j > 0) neigh += (cur >> (j - 1)) & 1U;
                        if (j < 4) neigh += (cur >> (j + 1)) & 1U;
                        /* below row */
                        if (j > 0) neigh += (nxt >> (j - 1)) & 1U;
                        neigh += (nxt >> j) & 1U;
                        if (j < 4) neigh += (nxt >> (j + 1)) & 1U;
                        if (neigh > 3) ++nbad;
                    }
                }
                if (nbad > 1)        bad_val[above][cur][nxt] = -1; /* invalid */
                else                 bad_val[above][cur][nxt] = (int8_t)nbad;
            }
        }
    }
    precomputed = true;
}

seq_t a(idx_t n)
{
    init_bad_val();
    if (n == 0) return 0;

    static seq_t dp[2][32][32], nextdp[2][32][32];

    /* clear dp */
    for (int b = 0; b < 2; ++b)
        for (int a = 0; a < 32; ++a)
            for (int c = 0; c < 32; ++c)
                dp[b][a][c] = 0;

    /* initial state: top padding row is all 0, first real row is any mask */
    for (int rowMask = 0; rowMask < 32; ++rowMask)
        dp[0][0][rowMask] = 1;

    /* process remaining rows 1 .. n-1 */
    for (idx_t step = 1; step < n; ++step) {
        /* clear nextdp */
        for (int b = 0; b < 2; ++b)
            for (int a = 0; a < 32; ++a)
                for (int c = 0; c < 32; ++c)
                    nextdp[b][a][c] = 0;

        for (int bad = 0; bad < 2; ++bad) {
            for (int aMask = 0; aMask < 32; ++aMask) {
                for (int cMask = 0; cMask < 32; ++cMask) {
                    seq_t ways = dp[bad][aMask][cMask];
                    if (ways == 0) continue;
                    for (int nMask = 0; nMask < 32; ++nMask) {
                        int8_t nb = bad_val[aMask][cMask][nMask];
                        if (nb < 0) continue;   /* invalid triple */
                        int newBad = bad + nb;
                        if (newBad > 1) continue; /* second violation not allowed */
                        nextdp[newBad][cMask][nMask] += ways;
                    }
                }
            }
        }
        /* swap dp and nextdp */
        for (int b = 0; b < 2; ++b)
            for (int a = 0; a < 32; ++a)
                for (int c = 0; c < 32; ++c)
                    dp[b][a][c] = nextdp[b][a][c];
    }

    /* finalize by adding the bottom padding row (all zeros) */
    seq_t result = 0;
    for (int bad = 0; bad < 2; ++bad) {
        for (int aMask = 0; aMask < 32; ++aMask) {
            for (int cMask = 0; cMask < 32; ++cMask) {
                seq_t ways = dp[bad][aMask][cMask];
                if (ways == 0) continue;
                int8_t nb = bad_val[aMask][cMask][0]; /* next row is all 0 */
                if (nb < 0) continue;                 /* invalid */
                int newBad = bad + nb;
                if (newBad > 1) continue;
                if (newBad == 1) result += ways;      /* exactly one violation */
            }
        }
    }
    return result;
}
