/*
 * A281954  Number of n X 7 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 0..1 arrays with * no element unequal to
 *            more than four of its king-move neighbors * and with new values
 *            introduced in order 0 sequentially upwards
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH        7
#define ROWS         128          /* 2^7 */
#define STATE_SIZE   (ROWS * ROWS)/* 16384 */

/* Transition lists: for each (prev,cur) pair, trans_list[prev][cur][k]
 * enumerates all rows that can follow cur and satisfy the
 * constraint for cur.  trans_len[prev][cur] gives the list length. */
static uint8_t trans_list[ROWS][ROWS][ROWS];
static uint8_t trans_len[ROWS][ROWS];

/* Index into the flat state array for a pair of rows (prev, cur). */
static inline int state_idx(int prev, int cur) {
    return (prev << WIDTH) | cur;
}

/* Helper: test whether a row is valid with optional above and below rows. */
static bool row_valid_mask(uint8_t row, bool have_above, bool have_below,
                           uint8_t above, uint8_t below)
{
    for (int col = 0; col < WIDTH; ++col) {
        int v = (row >> col) & 1;
        int diff = 0;
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nc = col + dc;
                if (nc < 0 || nc >= WIDTH) continue;
                unsigned char neigh_row;
                if (dr == -1) {
                    if (!have_above) continue;
                    neigh_row = above;
                } else if (dr == 1) {
                    if (!have_below) continue;
                    neigh_row = below;
                } else {
                    neigh_row = row;
                }
                int nv = (neigh_row >> nc) & 1;
                if (nv != v) {
                    ++diff;
                    if (diff > 4) return false;
                }
            }
        }
    }
    return true;
}

/* Precompute all transition lists. */
static void build_transitions(void)
{
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            uint8_t *list = trans_list[prev][cur];
            uint8_t cnt = 0;
            for (int next = 0; next < ROWS; ++next) {
                if (row_valid_mask(cur, true, true, (uint8_t)prev, (uint8_t)next))
                    list[cnt++] = (uint8_t)next;
            }
            trans_len[prev][cur] = cnt;
        }
    }
}

static bool trans_ready = false;

/* Memoised cache for the computed sequence values. */
static seq_t cache[20001];
static size_t cache_len = 0;
static size_t max_comp_n = 0;

/* DP arrays for counting. */
static seq_t dp_cur[STATE_SIZE];
static seq_t dp_next[STATE_SIZE];
static seq_t *dp_a = dp_cur;   /* current DP */
static seq_t *dp_b = dp_next;  /* next DP */

/* Compute the n-th term. */
seq_t a(idx_t n)
{
    if (!trans_ready) {
        build_transitions();
        trans_ready = true;
    }

    /* Compute up to n if necessary. */
    if (n > max_comp_n) {
        /* ---- initial values for n=1 and n=2 ---- */
        if (max_comp_n == 0) {
            /* a(1) */
            seq_t val1 = 0;
            for (int r = 0; r < ROWS; ++r) {
                if ((r & 1) == 0) {
                    if (row_valid_mask((uint8_t)r, false, false, 0, 0))
                        ++val1;
                }
            }
            cache[1] = val1;
            cache_len = 2;

            /* a(2) */
            seq_t val2 = 0;
            memset(dp_a, 0, sizeof(dp_cur));
            for (int r0 = 0; r0 < ROWS; ++r0) {
                if ((r0 & 1) == 0) {
                    for (int r1 = 0; r1 < ROWS; ++r1) {
                        if (row_valid_mask((uint8_t)r0, false, true, 0, (uint8_t)r1) &&
                            row_valid_mask((uint8_t)r1, true, false, (uint8_t)r0, 0)) {
                            dp_a[state_idx(r0, r1)] = 1;
                            ++val2;
                        }
                    }
                }
            }
            cache[2] = val2;
            max_comp_n = 2;
        }

        /* Extend computation up to n. */
        for (size_t target = max_comp_n + 1; target <= n; ++target) {
            /* Clear dp_b fully. */
            memset(dp_b, 0, sizeof(dp_next));

            /* Transition */
            for (int prev = 0; prev < ROWS; ++prev) {
                for (int cur = 0; cur < ROWS; ++cur) {
                    seq_t val = dp_a[state_idx(prev, cur)];
                    if (val == 0) continue;
                    uint8_t *list = trans_list[prev][cur];
                    uint8_t len   = trans_len[prev][cur];
                    for (uint8_t t = 0; t < len; ++t) {
                        int nxt = list[t];
                        dp_b[state_idx(cur, nxt)] += val;
                    }
                }
            }

            /* Compute a(target) by checking the bottom row. */
            seq_t ans = 0;
            for (int prev = 0; prev < ROWS; ++prev) {
                for (int cur = 0; cur < ROWS; ++cur) {
                    seq_t val = dp_b[state_idx(prev, cur)];
                    if (val == 0) continue;
                    if (row_valid_mask((uint8_t)cur, true, false,
                                       (uint8_t)prev, 0))
                        ans += val;
                }
            }

            cache[++max_comp_n] = ans;

            /* Swap dp arrays for next iteration. */
            seq_t *tmp = dp_a;
            dp_a = dp_b;
            dp_b = tmp;
        }
    }

    return cache[n];
}
