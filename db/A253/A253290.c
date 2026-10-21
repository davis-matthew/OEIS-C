/*
 * A253290  Number of (n+1)X(2+1) 0..2 arrays with every 2X2 subblock sum
 *          differing from its neighbors horizontally, vertically, diagonally
 *          and antidiagonally.
 * https://oeis.org/A253290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253290: Number of (n+1)×3 arrays with entries 0..2 such that every
 * 2×2 subblock sum differs from its horizontally, vertically,
 * diagonally, and antidiagonally adjacent subblocks.
 *
 * The function implements a dynamic‑programming algorithm over
 * consecutive pairs of rows.  Each row is a base‑3 number from 0 to 26
 * (there are 3³ rows).  A state is a pair (prevRow, curRow).  A
 * transition from (p, c) to (c, n) is allowed iff all adjacent
 * 2×2 sums of the block formed by rows p, c, n satisfy the distinctness
 * requirement.  Pre‑computing the allowed transitions makes each
 * step O(STATE) with STATE = 27×27 = 729.  The algorithm is
 * O(n) time and O(STATE) memory.  It works for any seq_t that
 * supports the integer operators and + for zeroing.
 */

#include <stdbool.h>

#define ROWS 27             /* 3^3 possible rows (0..26) */
#define STATE (ROWS * ROWS) /* 729 DP states (prevRow, curRow) */

/* Pre‑computed tables */
static unsigned char row_digits[ROWS][3];          /* base‑3 digits of a row */
static unsigned char sum_pair[ROWS][2];           /* sums of adjacent cells in a row */
static unsigned char blockSum[ROWS][ROWS][2];     /* sums of 2×2 blocks for two rows */
static bool horizontal_ok[ROWS][ROWS];           /* horizontal distinctness p,c */
static unsigned char next_rows[STATE][ROWS];      /* possible next rows for a state */
static unsigned char next_len[STATE];             /* number of valid next rows */

/* ------------------------------------------------------------------ */
/* Helper: initialise the tables once.                                */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    /* encode each row as base‑3 digits and compute row pair sums */
    for (int i = 0; i < ROWS; ++i) {
        int v = i;
        for (int k = 2; k >= 0; --k) {
            row_digits[i][k] = (unsigned char)(v % 3);
            v /= 3;
        }
        sum_pair[i][0] = row_digits[i][0] + row_digits[i][1];
        sum_pair[i][1] = row_digits[i][1] + row_digits[i][2];
    }

    /* pre‑compute block sums and horizontal checks for every pair of rows */
    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            unsigned char sum0 = row_digits[p][0] + row_digits[p][1]
                                 + row_digits[c][0] + row_digits[c][1];
            unsigned char sum1 = row_digits[p][1] + row_digits[p][2]
                                 + row_digits[c][1] + row_digits[c][2];
            blockSum[p][c][0] = sum0;
            blockSum[p][c][1] = sum1;
            horizontal_ok[p][c] = (sum0 != sum1);
        }
    }

    /* build transition lists for each state (p,c) */
    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            int state = p * ROWS + c;
            int len = 0;
            if (!horizontal_ok[p][c]) { /* state itself must be horizontal valid */
                next_len[state] = 0;
                continue;
            }
            for (int n = 0; n < ROWS; ++n) {
                if (!horizontal_ok[c][n]) continue;         /* horizontal c,n */

                /* vertical: sums of adjacent pairs must differ */
                if (sum_pair[n][0] == sum_pair[p][0]) continue;
                if (sum_pair[n][1] == sum_pair[p][1]) continue;

                /* diagonal and anti‑diagonal: block sums must differ */
                if (blockSum[c][n][0] == blockSum[p][c][1]) continue;
                if (blockSum[c][n][1] == blockSum[p][c][0]) continue;

                next_rows[state][len++] = (unsigned char)n;
            }
            next_len[state] = (unsigned char)len;
        }
    }
}

/* ------------------------------------------------------------------ */
/* The sequence function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise tables only once */
    static bool tables_init = false;
    if (!tables_init) {
        init_tables();
        tables_init = true;
    }

    /* DP arrays: dp[state] holds count of ways that end in state (p,c) */
    seq_t dp[STATE];
    seq_t newdp[STATE];

    /* zero initialise the arrays using a loop (important for big integers) */
    for (int i = 0; i < STATE; ++i) {
        dp[i] = 0;
        newdp[i] = 0;
    }

    /* initialisation: first two rows (row0,row1) must be horizontally distinct */
    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            if (!horizontal_ok[p][c]) continue;
            dp[p * ROWS + c] = 1;
        }
    }

    /* n=1 corresponds to a 2×3 array (just two rows) */
    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < STATE; ++s) total += dp[s];
        return total;
    }

    /* extend the array row by row up to n+1 rows total */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear newdp */
        for (int i = 0; i < STATE; ++i) newdp[i] = 0;

        /* propagate transitions */
        for (int state = 0; state < STATE; ++state) {
            seq_t val = dp[state];
            if (val == 0) continue;
            int c = state % ROWS;          /* current row index */
            int len = next_len[state];
            for (int idx = 0; idx < len; ++idx) {
                int nrow = next_rows[state][idx];
                int new_state = c * ROWS + nrow;
                newdp[new_state] += val;
            }
        }
        /* copy newdp to dp for next iteration */
        for (int i = 0; i < STATE; ++i) dp[i] = newdp[i];
    }

    /* sum all ending states */
    seq_t total = 0;
    for (int s = 0; s < STATE; ++s) total += dp[s];
    return total;
}
