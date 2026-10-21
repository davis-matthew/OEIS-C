/*
 * A241307  Number of 2Xn 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it or zero plus the sum of the elements diagonally to its
 *          northwest or zero plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A241307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP implementation for A241307 (see OEIS entry). The sequence is
 *            defined by the constraints described in the problem statement.
 *            This implementation follows the definition directly. No explicit
 *            recurrence formula is used; instead a linear–time DP over the
 *            columns is performed. The DP state contains...
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 78, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Number of possible states: 4^5 = 1024.  Index mapping:
       idx = (((lt*4 + lb)*4 + t_pprev)*4 + t_prev)*4 + b_prev  */
    const int STATE_COUNT = 1024;
    seq_t cur[STATE_COUNT];
    seq_t nxt[STATE_COUNT];

    /* Initialise all counts to zero */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = (seq_t)0;
    /* Initial state: all sums and previous values are zero */
    cur[0] = (seq_t)1;   /* index 0 corresponds to (lt=0,lb=0,t_pprev=0,t_prev=0,b_prev=0) */

    /* Process each column */
    for (idx_t col = 0; col < n; ++col) {
        /* Zero the next array */
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = (seq_t)0;

        for (int idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t cnt = cur[idx];
            if (cnt == (seq_t)0) continue;

            /* Decode the current state */
            int tmp = idx;
            int b_prev = tmp & 3;            tmp >>= 2;
            int t_prev = tmp & 3;            tmp >>= 2;
            int t_pprev = tmp & 3;           tmp >>= 2;
            int lb = tmp & 3;                tmp >>= 2;
            int lt = tmp & 3;

            /* Iterate over possible new top value */
            for (int t_cur = 0; t_cur < 4; ++t_cur) {
                /* Top cell constraints:
                   * not equal to 0 (NW), not equal 1 (above+1), not equal lt (left sum) */
                if (t_cur == 0 || t_cur == 1 || t_cur == lt) continue;

                /* Bottom constraint for previous column (col-1) applies only if we've
                   processed at least one column already (col >= 1). */
                int ok = 1;
                if (col >= 1) {
                    /* Left sum bottom before column (col-1) */
                    int left_bot_before = (lb + 4 - b_prev) & 3;

                    /* Bottom of previous column must differ from:
                       left_bot_before (sum to its left),
                       (t_prev + 1) mod 4 (above + 1),
                       t_pprev (NW),
                       t_cur      (NE) */
                    if (b_prev == left_bot_before ||
                        b_prev == ((t_prev + 1) & 3) ||
                        b_prev == t_pprev ||
                        b_prev == t_cur) {
                        ok = 0;
                    }
                }
                if (!ok) continue;

                /* Iterate over possible new bottom value (no immediate constraint) */
                for (int b_cur = 0; b_cur < 4; ++b_cur) {
                    int lt_next = (lt + t_cur) & 3;
                    int lb_next = (lb + b_cur) & 3;
                    int t_pprev_next = t_prev;
                    int t_prev_next = t_cur;
                    int b_prev_next = b_cur;
                    int next_idx = (((lt_next * 4 + lb_next) * 4 + t_pprev_next) * 4 + t_prev_next) * 4 + b_prev_next;
                    nxt[next_idx] = nxt[next_idx] + cnt;
                }
            }
        }
        /* Copy next state into current for the next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i];
    }

    /* After all columns processed, enforce the bottom constraint for the last column
       (NE is zero at the end). */
    seq_t result = (seq_t)0;
    for (int idx = 0; idx < STATE_COUNT; ++idx) {
        seq_t cnt = cur[idx];
        if (cnt == (seq_t)0) continue;

        /* Decode the final state */
        int tmp = idx;
        int b_prev = tmp & 3;            tmp >>= 2;
        int t_prev = tmp & 3;            tmp >>= 2;
        int t_pprev = tmp & 3;           tmp >>= 2;
        int lb = tmp & 3;                tmp >>= 2;
        int lt = tmp & 3;

        int left_bot_before = (lb + 4 - b_prev) & 3;

        /* Bottom of the last column must differ from:
           left_bot_before, (t_prev+1) mod 4, t_pprev (NW), and 0 (NE) */
        if (b_prev == left_bot_before ||
            b_prev == ((t_prev + 1) & 3) ||
            b_prev == t_pprev ||
            b_prev == 0) continue;

        result = result + cnt;
    }

    return result;
}
