/*
 * A283539  Number of n X 4 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Sequence A283539 by dynamic programming over row patterns.
 * The state is a pair of consecutive rows; a transition
 * (prev, curr) -> (curr, next) is allowed iff all 1‑cells in curr
 * have at most one neighbor among horizontal, diagonal, and
 * antidiagonal neighbors from prev and next rows.
 * The count for a given `n` is obtained by iterating this
 * transition over `n` rows and finally checking the last row
 * against a bottom sentinel row of zeros. */

#include <stdbool.h>

static bool valid[16][16][16];
static bool valid_inited = false;

static inline bool check_transition(unsigned prev, unsigned curr, unsigned next)
{
    for (int j = 0; j < 4; ++j) {
        if (!(curr & (1u << j))) continue;          /* current cell is 0 */
        int cnt = 0;
        /* horizontal */
        if (j > 0 && (curr & (1u << (j - 1)))) cnt++;
        if (j < 3 && (curr & (1u << (j + 1)))) cnt++;
        /* diagonal up-left / up-right */
        if (j > 0 && (prev & (1u << (j - 1)))) cnt++;
        if (j < 3 && (prev & (1u << (j + 1)))) cnt++;
        /* diagonal down-left / down-right */
        if (j > 0 && (next & (1u << (j - 1)))) cnt++;
        if (j < 3 && (next & (1u << (j + 1)))) cnt++;
        if (cnt > 1) return false;
    }
    return true;
}

static void init_valid(void)
{
    for (unsigned prev = 0; prev < 16; ++prev)
        for (unsigned curr = 0; curr < 16; ++curr)
            for (unsigned next = 0; next < 16; ++next)
                valid[prev][curr][next] = check_transition(prev, curr, next);
    valid_inited = true;
}

seq_t a(idx_t n)
{
    if (!valid_inited) init_valid();

    /* DP arrays: dp[prev*16 + curr] */
    seq_t dp[256] = {0};
    seq_t dp_next[256] = {0};

    /* initial state: before first row, prev=0, curr = any pattern */
    for (unsigned curr = 0; curr < 16; ++curr)
        dp[0 * 16 + curr] = 1;

    /* iterate from the second row up to nth row */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;
        /* transition */
        for (int old = 0; old < 256; ++old) {
            seq_t count = dp[old];
            if (count == 0) continue;
            unsigned prev = old / 16;
            unsigned curr = old % 16;
            for (unsigned next = 0; next < 16; ++next) {
                if (valid[prev][curr][next]) {
                    int idx = curr * 16 + next;
                    dp_next[idx] += count;
                }
            }
        }
        /* copy dp_next to dp */
        for (int i = 0; i < 256; ++i) dp[i] = dp_next[i];
    }

    /* final check for the last row against a bottom zero row */
    seq_t ans = 0;
    for (int idx = 0; idx < 256; ++idx) {
        unsigned prev = idx / 16;
        unsigned curr = idx % 16;
        if (valid[prev][curr][0]) {
            ans += dp[idx];
        }
    }
    return ans;
}
