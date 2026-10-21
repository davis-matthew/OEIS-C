/*
 * A297599  Number of 6Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 5 neighboring 1s.
 * https://oeis.org/A297599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A297599 – Number of 6×n binary arrays where every 1 has exactly 1 or 5 neighboring 1’s
    (neighbours are the cells that are horizontally, diagonally or antidiagonally adjacent).
    The function implements a direct dynamic‑programming enumeration over columns.
    O(64³) pre‑computation, then O(n·64²) time and O(64²) memory. */

#include <stdint.h>

/*  auxiliary data structures to be filled lazily  */
static int precomputed = 0;
static unsigned char valid_len[64][64];
static unsigned char valid_r[64][64][64];
static unsigned char single_valid[64];

/*  test whether pattern m satisfies the constraint when its left neighbour is l
    and its right neighbour is r  */
static int is_valid_triple(int l, int m, int r)
{
    for (int row = 0; row < 6; ++row) {
        if ((m >> row) & 1) {
            int neigh = 0;
            if ((l >> row) & 1)          ++neigh;
            if ((r >> row) & 1)          ++neigh;
            if (row > 0 && ((l >> (row-1)) & 1)) ++neigh;
            if (row < 5 && ((l >> (row+1)) & 1)) ++neigh;
            if (row > 0 && ((r >> (row-1)) & 1)) ++neigh;
            if (row < 5 && ((r >> (row+1)) & 1)) ++neigh;
            if (neigh != 1 && neigh != 5) return 0;
        }
    }
    return 1;
}

/*  main sequence function  */
seq_t a(idx_t n)
{
    /*  initialise helper tables only once  */
    if (!precomputed) {
        for (int l = 0; l < 64; ++l) {
            for (int m = 0; m < 64; ++m) {
                int len = 0;
                for (int r = 0; r < 64; ++r) {
                    if (is_valid_triple(l, m, r))
                        valid_r[l][m][len++] = (unsigned char)r;
                }
                valid_len[l][m] = (unsigned char)len;
            }
        }
        for (int m = 0; m < 64; ++m)
            single_valid[m] = is_valid_triple(0, m, 0) ? 1 : 0;
        precomputed = 1;
    }

    if (n == 0) return (seq_t)0;          /* offset is 1 – no value for n=0 */

    if (n == 1) {                         /* only the all‑zero column is valid  */
        seq_t res = 0;
        for (int m = 0; m < 64; ++m)
            if (single_valid[m])
                res += (seq_t)1;
        return res;
    }

    static seq_t dp[64][64];
    static seq_t dp_next[64][64];

    /*  initialise dp for the first two columns (0 and 1)  */
    for (int l = 0; l < 64; ++l)
        for (int m = 0; m < 64; ++m)
            dp[l][m] = 0;
    for (int a0 = 0; a0 < 64; ++a0) {
        for (int b = 0; b < 64; ++b) {
            if (is_valid_triple(0, a0, b))
                dp[a0][b] = dp[a0][b] + (seq_t)1;
        }
    }

    /*  add columns 2 … n-1  */
    for (idx_t col = 2; col < n; ++col) {
        for (int l = 0; l < 64; ++l)
            for (int m = 0; m < 64; ++m)
                dp_next[l][m] = 0;
        for (int l = 0; l < 64; ++l) {
            for (int m = 0; m < 64; ++m) {
                seq_t ways = dp[l][m];
                if (!ways) continue;
                unsigned char *rl = valid_r[l][m];
                int len = valid_len[l][m];
                for (int k = 0; k < len; ++k) {
                    int r = rl[k];
                    dp_next[m][r] = dp_next[m][r] + ways;
                }
            }
        }
        /*  copy dp_next to dp for next iteration  */
        for (int l = 0; l < 64; ++l)
            for (int m = 0; m < 64; ++m)
                dp[l][m] = dp_next[l][m];
    }

    /*  evaluate the last column (right neighbour absent)  */
    seq_t result = 0;
    for (int l = 0; l < 64; ++l) {
        for (int m = 0; m < 64; ++m) {
            seq_t ways = dp[l][m];
            if (!ways) continue;
            if (is_valid_triple(l, m, 0))    /* right neighbour = 0  */
                result = result + ways;
        }
    }
    return result;
}
