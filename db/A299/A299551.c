/*
 * A299551  Number of nX5 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A299551:
   number of n×5 binary arrays where each cell has 2,3,4,5,6 or 8 equal
   king‑move neighbours, with the upper left cell fixed to 0.
   The algorithm builds transition tables for two successive rows (5 bit
   patterns) and processes the rows in linear time. */
#include <stdint.h>
#include <stdbool.h>

#define WIDTH 5
#define STATE (1 << WIDTH)

/* Test whether a neighbour count is allowed. */
static inline unsigned char allowed_count(int c)
{
    return (c==2 || c==3 || c==4 || c==5 || c==6 || c==8);
}

/* Check whether row 'cur' satisfies the adjacency rule when the row
   above is 'up' and the row below is 'down'.
   upExists / downExists indicate the presence of the corresponding row. */
static unsigned char valid_row(int cur, int up, int down,
                               int upExists, int downExists)
{
    for (int col=0; col<WIDTH; ++col) {
        int val = (cur>>col) & 1;
        int count = 0;
        /* Upper neighbours */
        if (upExists) {
            if (col>0   && (((up>>(col-1)) & 1) == val)) count++;
            if          ((((up>>col)    & 1) == val)) count++;
            if (col<WIDTH-1 && (((up>>(col+1)) & 1) == val)) count++;
        }
        /* Same row neighbours */
        if (col>0   && (((cur>>(col-1)) & 1) == val)) count++;
        if (col<WIDTH-1 && (((cur>>(col+1)) & 1) == val)) count++;
        /* Lower neighbours */
        if (downExists) {
            if (col>0   && (((down>>(col-1)) & 1) == val)) count++;
            if          ((((down>>col)    & 1) == val)) count++;
            if (col<WIDTH-1 && (((down>>(col+1)) & 1) == val)) count++;
        }
        if (!allowed_count(count)) return 0;
    }
    return 1;
}

/* Transition tables – built once. */
static unsigned char nxt[STATE][STATE][STATE];
static unsigned char nxt_cnt[STATE][STATE];
static unsigned char top_valid[STATE][STATE];
static unsigned char bottom_valid[STATE][STATE];
static int tables_initialized = 0;

static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = 1;

    for (int up=0; up<STATE; ++up) {
        for (int cur=0; cur<STATE; ++cur) {
            unsigned int c = 0;
            for (int down=0; down<STATE; ++down) {
                if (valid_row(cur, up, down, 1, 1))
                    nxt[up][cur][c++] = (unsigned char)down;
            }
            nxt_cnt[up][cur] = (unsigned char)c;
        }
    }

    for (int cur=0; cur<STATE; ++cur)
        for (int down=0; down<STATE; ++down)
            top_valid[cur][down] = valid_row(cur, 0, down, 0, 1);

    for (int up=0; up<STATE; ++up)
        for (int cur=0; cur<STATE; ++cur)
            bottom_valid[up][cur] = valid_row(cur, up, 0, 1, 0);
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */
    init_tables();

    seq_t dp[STATE][STATE] = {0};

    /* initial two rows – enforce upper left element 0 */
    for (int r1=0; r1<STATE; ++r1) {
        if ((r1 & 1) == 0) {        /* upper left is 0 */
            for (int r2=0; r2<STATE; ++r2) {
                if (top_valid[r1][r2])
                    dp[r1][r2] = dp[r1][r2] + (seq_t)1;
            }
        }
    }

    /* process remaining rows, if any */
    for (idx_t row=3; row<=n; ++row) {
        seq_t next[STATE][STATE] = {0};

        for (int up=0; up<STATE; ++up) {
            for (int cur=0; cur<STATE; ++cur) {
                seq_t ways = dp[up][cur];
                if (!ways) continue;
                unsigned char cnt = nxt_cnt[up][cur];
                for (unsigned char k=0; k<cnt; ++k) {
                    int down = (int)nxt[up][cur][k];
                    next[cur][down] = next[cur][down] + ways;
                }
            }
        }
        /* copy next to dp for next iteration */
        for (int i=0; i<STATE; ++i)
            for (int j=0; j<STATE; ++j)
                dp[i][j] = next[i][j];
    }

    /* final row boundary check */
    seq_t result = (seq_t)0;
    for (int up=0; up<STATE; ++up) {
        for (int cur=0; cur<STATE; ++cur) {
            seq_t ways = dp[up][cur];
            if (!ways) continue;
            if (bottom_valid[up][cur])
                result = result + ways;
        }
    }
    return result;
}
