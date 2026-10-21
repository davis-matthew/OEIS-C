/*
 * A252322  Number of (n+2)X(1+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 3 or 6.
 * https://oeis.org/A252322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over all 3×(n+2) arrays
 *            whose entries are 0..3 and which avoid the forbidden 3×3
 *            subblock sums 3 or 6 in rows, columns, main diagonals, or
 *            antidiagonals. The algorithm runs in O(n·4096) time
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 17, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define STATES 4096          /* 64 × 64: all possible pairs of 3‑digit rows */

/* ------------------------------------------------------------ */
/* Helper data for each 3‑digit base‑4 row                       */
/* ------------------------------------------------------------ */
static unsigned char row_dig[64][3];   /* digit[0] is the least significant */
static unsigned char row_sum[64];      /* sum of the three digits          */

/* ------------------------------------------------------------ */
/* Transition tables:  from (r1,r2) we list admissible r3      */
/* ------------------------------------------------------------ */
static unsigned short adj[STATES][64];
static unsigned char nout[STATES];

/* ------------------------------------------------------------ */
/* Build the row data and the adjacency tables (once)           */
/* ------------------------------------------------------------ */
static bool adjacency_built = false;

static void build_row_data(void)
{
    for (unsigned int i = 0; i < 64; ++i) {
        unsigned int x = i;
        row_dig[i][0] =  x        & 3;
        row_dig[i][1] = (x >> 2)  & 3;
        row_dig[i][2] = (x >> 4)  & 3;
        row_sum[i]    = row_dig[i][0] + row_dig[i][1] + row_dig[i][2];
    }
}

static bool block_valid(unsigned int r1, unsigned int r2, unsigned int r3)
{
    /* Row sums must not be 3 or 6 */
    if (row_sum[r1] == 3 || row_sum[r1] == 6) return false;
    if (row_sum[r2] == 3 || row_sum[r2] == 6) return false;
    if (row_sum[r3] == 3 || row_sum[r3] == 6) return false;

    /* Column sums */
    unsigned char s;
    s = row_dig[r1][0] + row_dig[r2][0] + row_dig[r3][0];
    if (s == 3 || s == 6) return false;
    s = row_dig[r1][1] + row_dig[r2][1] + row_dig[r3][1];
    if (s == 3 || s == 6) return false;
    s = row_dig[r1][2] + row_dig[r2][2] + row_dig[r3][2];
    if (s == 3 || s == 6) return false;

    /* Main diagonal */
    s = row_dig[r1][0] + row_dig[r2][1] + row_dig[r3][2];
    if (s == 3 || s == 6) return false;
    /* Antidiagonal */
    s = row_dig[r1][2] + row_dig[r2][1] + row_dig[r3][0];
    if (s == 3 || s == 6) return false;

    return true;
}

static void build_adjacency(void)
{
    build_row_data();

    for (unsigned int r1 = 0; r1 < 64; ++r1) {
        for (unsigned int r2 = 0; r2 < 64; ++r2) {
            unsigned int state = r1 * 64 + r2;
            unsigned char cnt = 0;
            for (unsigned int r3 = 0; r3 < 64; ++r3) {
                if (block_valid(r1, r2, r3)) {
                    adj[state][cnt++] = (unsigned short)r3;
                }
            }
            nout[state] = cnt;
        }
    }
}

/* ------------------------------------------------------------ */
/* The sequence function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!adjacency_built) {
        build_adjacency();
        adjacency_built = true;
    }

    if (n == 0)                      /* offset is 1, but protect */
        return 0;

    static seq_t dp[STATES];
    static seq_t ndp[STATES];

    /* All initial pairs are admissible */
    for (size_t i = 0; i < STATES; ++i)
        dp[i] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (size_t i = 0; i < STATES; ++i)
            ndp[i] = 0;

        for (size_t i = 0; i < STATES; ++i) {
            seq_t cur = dp[i];
            if (cur == 0) continue;
            unsigned char cnt = nout[i];
            unsigned short *list = adj[i];
            unsigned int r2 = (unsigned int)(i % 64);   /* second row of the pair */
            for (unsigned char j = 0; j < cnt; ++j) {
                unsigned short r3 = list[j];
                size_t next_state = (size_t)r2 * 64 + r3;
                ndp[next_state] += cur;
            }
        }

        for (size_t i = 0; i < STATES; ++i)
            dp[i] = ndp[i];
    }

    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i)
        total += dp[i];

    return total;
}
