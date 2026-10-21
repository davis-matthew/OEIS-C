/*
 * A283541  Number of n X 6 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n × 6 binary arrays with no 1 equal to more than one
 *            of its horizontal, diagonal and antidiagonal neighbors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_STATES 4096

/* global tables */
static int  total_states = 0;
static int  r_of_state[MAX_STATES];
static int  s_of_state[MAX_STATES];
static int  trans_to[MAX_STATES][64];
static unsigned char trans_cnt[MAX_STATES];
static int  state_index[64][64];
static bool valid_row[64];
static int  hdeg_mask[64];
static bool initialized = false;

/* Build transition tables once.  */
static void init_trans(void)
{
    if (initialized)
        return;

    /* valid_row and horizontal neighbor masks */
    for (int p = 0; p < 64; ++p) {
        bool ok = true;
        int mask = 0;
        for (int j = 0; j < 6; ++j) {
            if ((p >> j) & 1) {
                bool left  = j > 0   && ((p >> (j-1)) & 1);
                bool right = j < 5   && ((p >> (j+1)) & 1);
                if (left && right) {   /* 111 pattern */
                    ok = false;
                    break;
                }
                if (left || right)
                    mask |= 1 << j;   /* this position has a horizontal neighbour */
            }
        }
        valid_row[p] = ok;
        hdeg_mask[p] = mask;
    }

    /* initialise state_index to -1 */
    for (int r = 0; r < 64; ++r)
        for (int s = 0; s < 64; ++s)
            state_index[r][s] = -1;

    /* build states: (r,s) where r is a valid row pattern
       and s is any submask of r (indicates which 1s already have a neighbour) */
    for (int r = 0; r < 64; ++r) {
        if (!valid_row[r])
            continue;
        for (int s = r; ; s = (s - 1) & r) {
            int idx = total_states++;
            r_of_state[idx] = r;
            s_of_state[idx] = s;
            state_index[r][s] = idx;
            if (s == 0)
                break;
        }
    }

    /* precompute all transitions */
    for (int from = 0; from < total_states; ++from) {
        int r = r_of_state[from];
        int s = s_of_state[from];
        for (int c = 0; c < 64; ++c) {
            if (!valid_row[c])
                continue;

            bool ok = true;

            /* check constraints for 1s in previous row r */
            for (int j = 0; j < 6 && ok; ++j) {
                if ((r >> j) & 1) {
                    bool down_left  = (j > 0)   && ((c >> (j-1)) & 1);
                    bool down_right = (j < 5)   && ((c >> (j+1)) & 1);
                    bool already    = (s >> j) & 1;
                    if (already) {
                        if (down_left || down_right)
                            ok = false;
                    } else {
                        if (down_left && down_right)
                            ok = false;
                    }
                }
            }

            /* check constraints for 1s in current row c */
            for (int j = 0; j < 6 && ok; ++j) {
                if ((c >> j) & 1) {
                    bool horiz   = (hdeg_mask[c] >> j) & 1;
                    bool up_left   = (j > 0)   && ((r >> (j-1)) & 1);
                    bool up_right  = (j < 5)   && ((r >> (j+1)) & 1);
                    if (horiz) {
                        if (up_left || up_right)
                            ok = false;
                    } else {
                        if (up_left && up_right)
                            ok = false;
                    }
                }
            }

            if (!ok)
                continue;

            /* compute new status mask for row c */
            int new_s = 0;
            for (int j = 0; j < 6; ++j) {
                if ((c >> j) & 1) {
                    bool used = (hdeg_mask[c] >> j) & 1;          /* horizontal */
                    bool up_left  = (j > 0)   && ((r >> (j-1)) & 1);
                    bool up_right = (j < 5)   && ((r >> (j+1)) & 1);
                    if (up_left || up_right)
                        used = true;            /* neighbor from above */
                    if (used)
                        new_s |= 1 << j;
                }
            }

            int to = state_index[c][new_s];
            trans_to[from][trans_cnt[from]++] = to;
        }
    }

    initialized = true;
}

/* DP over rows */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_trans();

    static seq_t dp[MAX_STATES];
    static seq_t dp_next[MAX_STATES];

    /* initialise dp for first row */
    for (int i = 0; i < total_states; ++i)
        dp[i] = 0;

    for (int c = 0; c < 64; ++c) {
        if (!valid_row[c])
            continue;
        int s0 = hdeg_mask[c];
        int st = state_index[c][s0];
        dp[st] += (seq_t)1;
    }

    /* remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < total_states; ++i)
            dp_next[i] = 0;

        for (int i = 0; i < total_states; ++i) {
            seq_t cur = dp[i];
            if (cur == 0)
                continue;
            unsigned char cnt = trans_cnt[i];
            int *tptr = trans_to[i];
            for (int k = 0; k < cnt; ++k) {
                int to = tptr[k];
                dp_next[to] += cur;
            }
        }

        for (int i = 0; i < total_states; ++i)
            dp[i] = dp_next[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < total_states; ++i)
        ans += dp[i];
    return ans;
}
