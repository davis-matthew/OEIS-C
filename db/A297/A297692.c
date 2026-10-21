/*
 * A297692  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A297692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by dynamic programming over the n rows of a
 * 6‑column binary matrix.  For every row we keep the last two rows,
 * and we precompute for each pair of rows (a,b) every feasible next row c.
 * The answer is the number of n‑row sequences ending with rows (a,b)
 * that can transition to a dummy row of all zeros.  This implements
 * exactly the definition given in OEIS A297692. */

#include <stdbool.h>

/* Test whether the middle row 'b' is valid when its upper neighbour
   is 'a' and its lower neighbour is 'c'.
   For each 1 in 'b', its neighbours are:
     - left/right in 'b'
     - up-left/up-right in 'a'
     - down-left/down-right in 'c'
   The number of neighbouring 1s must be 0, 2 or 4. */
static bool valid_transition(unsigned a, unsigned b, unsigned c)
{
    for (int j = 0; j < 6; ++j) {
        if ((b >> j) & 1U) {
            int cnt = 0;
            /* horizontal neighbours */
            if (j > 0)   cnt += (b >> (j - 1)) & 1U;
            if (j < 5)   cnt += (b >> (j + 1)) & 1U;
            /* diagonal neighbours */
            if (j > 0)   cnt += (a >> (j - 1)) & 1U;   /* up-left  */
            if (j < 5)   cnt += (a >> (j + 1)) & 1U;   /* up-right */
            if (j > 0)   cnt += (c >> (j - 1)) & 1U;   /* down-left  */
            if (j < 5)   cnt += (c >> (j + 1)) & 1U;   /* down-right */
            if (!(cnt == 0 || cnt == 2 || cnt == 4))
                return false;
        }
    }
    return true;
}

/* Transition lookup tables, built once. */
static unsigned char trans[64][64][64];
static unsigned char nextCnt[64][64];
static unsigned char nextList[64][64][64];
static bool transition_built = false;

/* Build the transition tables. */
static void build_transition_tables(void)
{
    for (unsigned a = 0; a < 64; ++a) {
        for (unsigned b = 0; b < 64; ++b) {
            unsigned char k = 0;
            for (unsigned c = 0; c < 64; ++c) {
                if (valid_transition(a, b, c)) {
                    trans[a][b][c] = 1;
                    nextList[a][b][k++] = (unsigned char)c;
                } else {
                    trans[a][b][c] = 0;
                }
            }
            nextCnt[a][b] = k;
        }
    }
    transition_built = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!transition_built) build_transition_tables();

    /* dp[a][b] = number of ways ending with rows (a,b) */
    seq_t dp[64][64];
    seq_t ndp[64][64];

    /* initialise: top dummy row = 0, first real row = r */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;
    for (int r = 0; r < 64; ++r)
        dp[0][r] = 1;

    /* Special case n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 64; ++r)
            if (trans[0][r][0])
                ans += dp[0][r];
        return ans;
    }

    /* Process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear ndp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                ndp[i][j] = 0;

        /* transition */
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t val = dp[a][b];
                if (!val) continue;
                int k = nextCnt[a][b];
                for (int t = 0; t < k; ++t) {
                    int c = nextList[a][b][t];
                    ndp[b][c] += val;
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final row must transition to dummy row of zeros */
    seq_t ans = 0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            seq_t val = dp[a][b];
            if (!val) continue;
            if (trans[a][b][0])
                ans += val;
        }
    }
    return ans;
}
