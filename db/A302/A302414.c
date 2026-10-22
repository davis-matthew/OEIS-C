/*
 * A302414  Number of nX7 0..1 arrays with every element equal to 0, 1, 2 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays where each element has 0, 1,
 *            2 or 5 equal neighbours horizontally, diagonally or
 *            anti‑diagonally (no vertical neighbours), with the upper‑left
 *            element fixed to 0
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* helper: check a row for the adjacency rule                         */
/* ------------------------------------------------------------------ */
static inline bool row_ok(uint8_t mid, uint8_t upper, bool up_present,
                          uint8_t lower, bool lo_present)
{
    for (int col = 0; col < 7; ++col) {
        uint8_t val_mid = (mid >> col) & 1;
        if (!up_present) {
            if (lo_present && ((lower >> col) & 1) == val_mid)
                ; /* vertical not considered */
        }
        int cnt = 0;
        /* horizontal neighbours in the same row */
        if (col > 0 && ((mid >> (col - 1)) & 1) == val_mid) cnt++;
        if (col < 6 && ((mid >> (col + 1)) & 1) == val_mid) cnt++;
        /* diagonal neighbours from upper row */
        if (up_present) {
            if (col > 0 && ((upper >> (col - 1)) & 1) == val_mid) cnt++;
            if (col < 6 && ((upper >> (col + 1)) & 1) == val_mid) cnt++;
        }
        /* diagonal neighbours from lower row */
        if (lo_present) {
            if (col > 0 && ((lower >> (col - 1)) & 1) == val_mid) cnt++;
            if (col < 6 && ((lower >> (col + 1)) & 1) == val_mid) cnt++;
        }
        if (cnt != 0 && cnt != 1 && cnt != 2 && cnt != 5)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* pre‑computed transition tables                                      */
/* ------------------------------------------------------------------ */
static uint8_t trans_next[128][128][128];
static uint8_t trans_count[128][128];
static bool     trans_ready = false;

static void build_transitions(void)
{
    if (trans_ready) return;
    for (int prev = 0; prev < 128; ++prev) {
        for (int cur = 0; cur < 128; ++cur) {
            uint8_t cnt = 0;
            for (int next = 0; next < 128; ++next) {
                if (row_ok((uint8_t)cur, (uint8_t)prev, true,
                           (uint8_t)next, true))
                    trans_next[prev][cur][cnt++] = (uint8_t)next;
            }
            trans_count[prev][cur] = cnt;
        }
    }
    trans_ready = true;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    if (n == 1) return 64;         /* all 6 free bits with upper‑left 0 */
    if (n == 2) {
        seq_t ans = 0;
        for (int r1 = 0; r1 < 128; ++r1) {
            if (r1 & 1) continue;           /* upper‑left must be 0 */
            for (int r2 = 0; r2 < 128; ++r2) {
                if (row_ok((uint8_t)r1, 0, false,
                           (uint8_t)r2, true) &&
                    row_ok((uint8_t)r2, (uint8_t)r1, true,
                           0, false))
                    ans += (seq_t)1;
            }
        }
        return ans;
    }

    /* n >= 3 */
    build_transitions();

    static seq_t dp[128][128];
    static seq_t newdp[128][128];
    memset(dp, 0, sizeof(dp));

    /* initialise with the first two rows */
    for (int r1 = 0; r1 < 128; ++r1) {
        if (r1 & 1) continue;          /* upper‑left bit = 0 */
        for (int r2 = 0; r2 < 128; ++r2) {
            if (row_ok((uint8_t)r1, 0, false,
                       (uint8_t)r2, true))
                dp[r1][r2] += (seq_t)1;
        }
    }

    /* add rows 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(newdp, 0, sizeof(newdp));
        for (int prev = 0; prev < 128; ++prev) {
            for (int cur = 0; cur < 128; ++cur) {
                seq_t val = dp[prev][cur];
                if (!val) continue;
                uint8_t cnt = trans_count[prev][cur];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint8_t next = trans_next[prev][cur][k];
                    newdp[cur][next] += val;
                }
            }
        }
        /* copy newdp back to dp for next iteration */
        memcpy(dp, newdp, sizeof(dp));
    }

    /* final row has no lower neighbour */
    seq_t ans = 0;
    for (int prev = 0; prev < 128; ++prev) {
        for (int cur = 0; cur < 128; ++cur) {
            seq_t val = dp[prev][cur];
            if (!val) continue;
            if (row_ok((uint8_t)cur, (uint8_t)prev, true,
                       0, false))
                ans += val;
        }
    }

    return ans;
}
