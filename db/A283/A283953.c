/*
 * A283953  Number of 4Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A283953: number of 4×n binary arrays with exactly one
   1 that has more than three horizontal, diagonal, or antidiagonal
   neighbours (vertical neighbours are NOT counted).  The computation is
   performed by a dynamic program over a sliding window of three
   consecutive column masks. */

#include <stdbool.h>

/* Pre‑compute the number of violating cells in the middle column of a
   triple of consecutive columns (left mask, middle mask, right mask).
   A cell in the middle column violates if it has more than three of
   its six possible neighbours (horizontal + two diagonals on each side)
   set to 1. */
static bool bad_precomputed = false;
static unsigned char bad_count[16][16][16];

static void initialize_bad_count(void)
{
    if (bad_precomputed)
        return;
    for (int l = 0; l < 16; ++l) {
        for (int m = 0; m < 16; ++m) {
            for (int r = 0; r < 16; ++r) {
                unsigned char bad = 0;
                for (int j = 0; j < 4; ++j) {
                    if (!(m & (1 << j)))
                        continue;           /* cell is 0 – never violates */
                    unsigned char cnt = 0;
                    /* left / right in same row */
                    if (l & (1 << j)) cnt++;
                    if (r & (1 << j)) cnt++;
                    /* diagonal left‑up / left‑down */
                    if (j > 0   && (l & (1 << (j - 1)))) cnt++;
                    if (j < 3   && (l & (1 << (j + 1)))) cnt++;
                    /* diagonal right‑up / right‑down */
                    if (j > 0   && (r & (1 << (j - 1)))) cnt++;
                    if (j < 3   && (r & (1 << (j + 1)))) cnt++;
                    /* vertical neighbours are NOT counted */
                    if (cnt > 3)
                        bad++;
                }
                bad_count[l][m][r] = bad;
            }
        }
    }
    bad_precomputed = true;
}

seq_t a(idx_t n)
{
    /* Safety net for n == 0, though the OEIS offset starts at 1. */
    if (n == 0)
        return (seq_t)0;

    initialize_bad_count();

    /* DP state: cur[l][m][k] is the number of ways to reach the
       current column with left column mask l, current column mask m,
       and k violations so far (k = 0 or 1). */
    static seq_t cur[16][16][2];
    static seq_t nxt[16][16][2];

    /* Clear previous state */
    for (int l = 0; l < 16; ++l)
        for (int m = 0; m < 16; ++m)
            for (int k = 0; k < 2; ++k)
                cur[l][m][k] = (seq_t)0;

    /* Initialise with the first column; there is no left neighbour. */
    for (int m = 0; m < 16; ++m)
        cur[0][m][0] = (seq_t)1;

    /* Process columns 2 .. n */
    for (idx_t pos = 1; pos < n; ++pos) {
        /* Clear next state */
        for (int l = 0; l < 16; ++l)
            for (int m = 0; m < 16; ++m)
                for (int k = 0; k < 2; ++k)
                    nxt[l][m][k] = (seq_t)0;

        /* Transition */
        for (int l = 0; l < 16; ++l) {
            for (int m = 0; m < 16; ++m) {
                for (int k = 0; k < 2; ++k) {
                    seq_t ways = cur[l][m][k];
                    if (ways == (seq_t)0)
                        continue;
                    for (int r = 0; r < 16; ++r) {
                        unsigned char bad = bad_count[l][m][r];
                        int nk = k + bad;
                        if (nk <= 1)
                            nxt[m][r][nk] += ways;
                    }
                }
            }
        }

        /* Swap cur and nxt for next iteration */
        for (int l = 0; l < 16; ++l)
            for (int m = 0; m < 16; ++m)
                for (int k = 0; k < 2; ++k)
                    cur[l][m][k] = nxt[l][m][k];
    }

    /* Final evaluation of the last column (right neighbour is 0) */
    seq_t total = (seq_t)0;
    for (int l = 0; l < 16; ++l) {
        for (int m = 0; m < 16; ++m) {
            for (int k = 0; k < 2; ++k) {
                seq_t ways = cur[l][m][k];
                if (ways == (seq_t)0)
                    continue;
                unsigned char bad = bad_count[l][m][0];   /* right neighbour is 0 */
                int nk = k + bad;
                if (nk == 1)
                    total += ways;
            }
        }
    }
    return total;
}
