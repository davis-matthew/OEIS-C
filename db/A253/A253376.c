/*
 * A253376  Number of (2+1) X (n+1) 0..2 arrays with every 2 X 2 subblock sum
 *          nondecreasing horizontally, vertically and antidiagonally
 *          ne-to-sw.
 * https://oeis.org/A253376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×(n+1) 0..2 arrays whose every 2×2 subblock
 *            sum is nondecreasing horizontally, vertically and
 *            anti‑diagonally (NE→SW). The function implements a
 *            dynamic‑programming enumeration of column configurations
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 28, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* constants ---------------------------------------------------------*/
#define COLS      27              /* 3‑digit base‑3 columns: 0…26 */
#define SUMR      9               /* possible block sums: 0…8 */
#define MAX_TRANS COLS            /* each state can transition to at most one column per choice */
#define STATE_COUNT (COLS * SUMR * SUMR)   /* 27×9×9 = 2187 */

/* encode a state (col, top_sum, bottom_sum) ------------------------*/
static inline idx_t encode_state(idx_t col, idx_t sum0, idx_t sum1)
{
    return ((col * SUMR) + sum0) * SUMR + sum1;
}

/* transition tables ------------------------------------------------*/
static int  next_len[STATE_COUNT];
static idx_t next_state[STATE_COUNT][MAX_TRANS];

/* pre‑computed column digit values and block sums -----------------*/
static int col_vals[COLS][3];
static int sum0[COLS][COLS];   /* top block sum of pair (a,b) */
static int sum1[COLS][COLS];   /* bottom block sum of pair (a,b) */

/* DP storage -------------------------------------------------------*/
static seq_t dp_storage[2][STATE_COUNT];
static seq_t *dp_cur, *dp_next;

/* initialization flag ---------------------------------------------*/
static bool init = false;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* outside OEIS offset */

    /* one‑time setup ---------------------------------------------*/
    if (!init) {
        /* column digits (base 3 representations) */
        for (idx_t col = 0; col < COLS; ++col) {
            int t = (int)col;
            col_vals[col][0] = t % 3; t /= 3;
            col_vals[col][1] = t % 3; t /= 3;
            col_vals[col][2] = t % 3;
        }

        /* block sums for every ordered column pair */
        for (idx_t a = 0; a < COLS; ++a) {
            for (idx_t b = 0; b < COLS; ++b) {
                int a0 = col_vals[a][0], a1 = col_vals[a][1], a2 = col_vals[a][2];
                int b0 = col_vals[b][0], b1 = col_vals[b][1], b2 = col_vals[b][2];
                sum0[a][b] = a0 + a1 + b0 + b1;   /* top 2 rows */
                sum1[a][b] = a1 + a2 + b1 + b2;   /* bottom 2 rows */
            }
        }

        /* compute transition list for every state */
        for (idx_t col_prev = 0; col_prev < COLS; ++col_prev) {
            for (idx_t s0_prev = 0; s0_prev < SUMR; ++s0_prev) {
                for (idx_t s1_prev = 0; s1_prev < SUMR; ++s1_prev) {
                    idx_t state = encode_state(col_prev, s0_prev, s1_prev);
                    int len = 0;
                    for (idx_t col_next = 0; col_next < COLS; ++col_next) {
                        int s0_new = sum0[col_prev][col_next];
                        int s1_new = sum1[col_prev][col_next];
                        /* constraints:
                           horizontal (top & bottom),
                           vertical (in new block),
                           diagonal NE→SW: top_new <= bottom_prev */
                        if (s0_prev <= s0_new &&
                            s1_prev <= s1_new &&
                            s0_new <= s1_prev &&   /* diagonal */
                            s0_new <= s1_new) {    /* vertical */
                            idx_t nxt = encode_state(col_next, s0_new, s1_new);
                            next_state[state][len++] = nxt;
                        }
                    }
                    next_len[state] = len;
                }
            }
        }

        /* DP pointers */
        dp_cur  = dp_storage[0];
        dp_next = dp_storage[1];
        init = true;
    }

    /* reset DP for this n -----------------------------------------*/
    for (idx_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;

    /* initial configuration: first block between columns 1 and 2 */
    for (idx_t col0 = 0; col0 < COLS; ++col0) {
        for (idx_t col1 = 0; col1 < COLS; ++col1) {
            int s0 = sum0[col0][col1];
            int s1 = sum1[col0][col1];
            if (s0 <= s1) {
                idx_t state = encode_state(col1, (idx_t)s0, (idx_t)s1);
                dp_cur[state] += (seq_t)1;
            }
        }
    }

    /* extend columns step by step ----------------------------------*/
    idx_t steps = n - 1;           /* n ≥ 1, unsigned so safe */
    for (idx_t step = 1; step <= steps; ++step) {
        for (idx_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (idx_t state = 0; state < STATE_COUNT; ++state) {
            seq_t val = dp_cur[state];
            if (val == (seq_t)0) continue;
            int len = next_len[state];
            for (int k = 0; k < len; ++k) {
                idx_t nxt = next_state[state][k];
                dp_next[nxt] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;
    }

    /* sum all final states ----------------------------------------*/
    seq_t result = 0;
    for (idx_t i = 0; i < STATE_COUNT; ++i) result += dp_cur[i];
    return result;
}
