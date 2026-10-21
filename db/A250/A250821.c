/*
 * A250821  Number of (n+2)X(2+2) 0..1 arrays with nondecreasing maximum minus
 *          minimum of every three consecutive values in every row and column.
 * https://oeis.org/A250821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computation by dynamic programming over rows. * Horizontal
 *            constraint: for each row r = (b0,b1,b2,b3), *
 *            max(b0,b1,b2)-min(b0,b1,b2) <= max(b1,b2,b3)-min(b1,b2,b3) *
 *            Vertical constraint: for each column j and for all i, *
 *            w_{i+1,j} >= w_{i,j} where
 *            w_{i,j}=max(b_{i,j},b_{i+1,j},b_{i+2,j})- * min(...
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t ALL_ROWS[16] = {
    0x0, 0x1, 0x2, 0x3,
    0x4, 0x5, 0x6, 0x7,
    0x8, 0x9, 0xA, 0xB,
    0xC, 0xD, 0xE, 0xF
};

static int allowed_rows[16];
static int allowed_cnt;
static uint8_t triple_w[16][16][16];
static int precomp_done = 0;

static int row_valid(uint8_t r)
{
    int b0 = (r >> 0) & 1;
    int b1 = (r >> 1) & 1;
    int b2 = (r >> 2) & 1;
    int b3 = (r >> 3) & 1;

    int w1 = (b0 == b1 && b1 == b2) ? 0 : 1;     // max-min of (b0,b1,b2)
    int w2 = (b1 == b2 && b2 == b3) ? 0 : 1;     // max-min of (b1,b2,b3)

    return !(w1 && !w2);   // w1=1 and w2=0 not allowed
}

static void init_precomp(void)
{
    int i, j, k, col;
    for (i = 0; i < 16; ++i) {
        if (!row_valid((uint8_t)i)) continue;
        allowed_rows[allowed_cnt++] = i;
    }

    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            for (k = 0; k < 16; ++k) {
                uint8_t mask = 0;
                for (col = 0; col < 4; ++col) {
                    int a = (i >> col) & 1;
                    int b = (j >> col) & 1;
                    int c = (k >> col) & 1;
                    int all_same = (a == b && b == c);
                    if (!all_same) mask |= (1U << col);
                }
                triple_w[i][j][k] = mask;
            }
        }
    }
    precomp_done = 1;
}

seq_t a(idx_t n)
{
    if (!precomp_done)
        init_precomp();

    idx_t m = n + (idx_t)2;          /* number of rows */
    if (m < 3) {                    /* n==0 case, not used by OEIS */
        seq_t zero = 0;
        return zero;
    }

    /* DP state: (prev_row, curr_row, w_prev_bits) packed into 12 bits.
     * 0 <= prev_row < 16, 0 <= curr_row < 16, 0 <= w_prev_bits < 16.
     */
    enum { STATE_SIZE = 4096 };     /* 16*16*16 */

    static seq_t dp_cur[STATE_SIZE];
    static seq_t dp_next[STATE_SIZE];

    /* initialize dp for rows 1 and 2 */
    for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = 0;
    for (int r1 = 0; r1 < 16; ++r1) {
        if (!row_valid((uint8_t)r1)) continue;
        for (int r2 = 0; r2 < 16; ++r2) {
            if (!row_valid((uint8_t)r2)) continue;
            uint16_t key = ((uint16_t)r1 << 8) | ((uint16_t)r2 << 4); /* wprev = 0 */
            dp_cur[key] += (seq_t)1;
        }
    }

    /* iterate over rows 3 .. m */
    for (idx_t pos = 3; pos <= m; ++pos) {
        for (int i = 0; i < STATE_SIZE; ++i) dp_next[i] = 0;

        for (int state = 0; state < STATE_SIZE; ++state) {
            seq_t ways = dp_cur[state];
            if (!ways) continue;

            int prev_row = (state >> 8) & 0xF;
            int curr_row = (state >> 4) & 0xF;
            int wprev = state & 0xF;

            for (int idx = 0; idx < allowed_cnt; ++idx) {
                int next_row = allowed_rows[idx];
                uint8_t w_new = triple_w[prev_row][curr_row][next_row];

                /* check monotonic: w_prev <= w_new bitwise */
                if ((wprev & ((uint8_t)~w_new)) != 0) continue;

                uint16_t new_state = ((uint16_t)curr_row << 8)
                                  | ((uint16_t)next_row << 4)
                                  |  w_new;
                dp_next[new_state] += ways;
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all remaining states: result = total number of arrays */
    seq_t result = 0;
    for (int i = 0; i < STATE_SIZE; ++i)
        result += dp_cur[i];

    return result;
}
