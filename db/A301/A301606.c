/*
 * A301606  Number of nX6 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301606
 * Number of n×6 0..1 arrays with every element equal to 0, 2 or 3
 * horizontally or vertically adjacent elements, with upper‑left element zero.
 * The problem is solved by a transfer‑matrix dynamic‑programming approach.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Check whether a row `curr` satisfies the neighbour rule when
   its upper neighbour is `prev` (if present) and its lower neighbour
   is `nxt` (if present). */
static inline bool row_valid(uint8_t curr, uint8_t prev, uint8_t nxt,
                             bool up_present, bool down_present)
{
    for (int j = 0; j < 6; ++j) {
        int val = (curr >> j) & 1;
        int cnt = 0;
        if (up_present   && ((prev   >> j) & 1) == val) ++cnt;
        if (down_present && ((nxt    >> j) & 1) == val) ++cnt;
        if (j > 0        && ((curr >> (j-1)) & 1) == val) ++cnt;
        if (j < 5        && ((curr >> (j+1)) & 1) == val) ++cnt;
        if (cnt && cnt != 2 && cnt != 3) return false;
    }
    return true;
}

/* Pre‑computed admissible next rows for each pair (prev,curr).
   allowed_d[p][c] contains all rows d such that
   row_valid(c,p,d,true,true) holds; allowed_cnt[p][c] is their count. */
static bool precomp_done = false;
static uint8_t allowed_d[64][64][64];
static uint8_t allowed_cnt[64][64];

static void precompute(void)
{
    if (precomp_done) return;
    for (int p = 0; p < 64; ++p) {
        for (int c = 0; c < 64; ++c) {
            uint8_t cnt = 0;
            for (int d = 0; d < 64; ++d) {
                if (row_valid((uint8_t)c, (uint8_t)p, (uint8_t)d,
                              true, true))
                {
                    allowed_d[p][c][cnt++] = (uint8_t)d;
                }
            }
            allowed_cnt[p][c] = cnt;
        }
    }
    precomp_done = true;
}

/* DP working arrays – two 64×64 tables of `seq_t`. */
static seq_t dp_arr1[64 * 64];
static seq_t dp_arr2[64 * 64];

seq_t a(idx_t n)
{
    precompute();

    if (n == 0) return 0;           /* undefined, but safe */

    /* Special case n == 1 – only one row exists. */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t row0 = 0; row0 < 64; ++row0) {
            if (row0 & 1) continue;                 /* upper‑left must be 0 */
            if (row_valid(row0, 0, 0, false, false))
                ans += 1;
        }
        return ans;
    }

    const int STATES = 64 * 64;

    /* Pointers to the current and next DP tables. */
    seq_t *dp_cur = dp_arr1;
    seq_t *dp_next = dp_arr2;

    /* Initialise dp_cur for the first two rows. */
    for (int i = 0; i < STATES; ++i) dp_cur[i] = 0;
    for (uint8_t row0 = 0; row0 < 64; ++row0) {
        if (row0 & 1) continue;            /* top‑left must be 0 */
        for (uint8_t row1 = 0; row1 < 64; ++row1) {
            if (row_valid(row0, 0, row1, false, true))
                ++dp_cur[(row0 << 6) | row1];
        }
    }

    /* Process rows 2 .. n-1 (0‑based indexing). */
    for (idx_t step = 2; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;
        for (int idx = 0; idx < STATES; ++idx) {
            seq_t val = dp_cur[idx];
            if (!val) continue;
            int p   = idx >> 6;
            int c   = idx & 63;
            uint8_t *list = allowed_d[p][c];
            uint8_t   cnt  = allowed_cnt[p][c];
            for (uint8_t ki = 0; ki < cnt; ++ki) {
                int d = list[ki];
                dp_next[(c << 6) | d] += val;
            }
        }
        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;
    }

    /* Final row should not have a lower neighbour. */
    seq_t ans = 0;
    for (int idx = 0; idx < STATES; ++idx) {
        seq_t val = dp_cur[idx];
        if (!val) continue;
        int p = idx >> 6;
        int c = idx & 63;
        if (row_valid((uint8_t)c, (uint8_t)p, 0, true, false))
            ans += val;
    }
    return ans;
}
