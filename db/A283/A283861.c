/*
 * A283861  Number of 5Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays in which every 1 has at most
 *            three of its six possible neighbors (left, right, up‑left,
 *            down‑right, up‑right, down‑left) equal to 1. The computation
 *            uses a linear DP with states given by two consecutive columns
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   cur_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/cur_is_valid.h"

/* Pre‑computed transition lists.  nexts[i][k] gives the k‑th allowed
   next column for state i = prev*32 + cur.  next_count[i] is the
   number of allowed nexts. */
static uint8_t nexts[1024][32];
static uint8_t next_count[1024];
static bool nexts_ready = false;

/* Initialise transition tables on first call. */
static void init_transitions(void)
{
    if (nexts_ready) return;
    for (uint32_t prev = 0; prev < 32; ++prev) {
        for (uint32_t cur = 0; cur < 32; ++cur) {
            uint32_t idx = (prev << 5) | cur;
            uint8_t cnt = 0;
            for (uint32_t next = 0; next < 32; ++next) {
                if (cur_is_valid(prev, cur, (uint32_t)next)) {
                    nexts[idx][cnt++] = (uint8_t)next;
                }
            }
            next_count[idx] = cnt;
        }
    }
    nexts_ready = true;
}

/* The main sequence function required for the build. */
seq_t a(idx_t n)
{
    /* Avoid underflow if n < 1; the sequence is defined for n >= 1. */
    if (n < 1) return (seq_t)0;

    init_transitions();

    /* DP arrays of size 1024 (32×32 states). */
    seq_t dp[1024] = {0};
    seq_t ndp[1024] = {0};

    /* Initial state: prev column = 0 (virtual all‑zero), current = any column. */
    for (uint32_t cur = 0; cur < 32; ++cur) {
        dp[0 * 32 + cur] = (seq_t)1;
    }

    /* Process columns 2 .. n.  Each step adds one new column. */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero ndp. */
        for (size_t i = 0; i < 1024; ++i) ndp[i] = 0;

        for (uint32_t state = 0; state < 1024; ++state) {
            if (dp[state] == 0) continue;
            seq_t ways = dp[state];
            uint8_t cnt = next_count[state];
            for (uint8_t k = 0; k < cnt; ++k) {
                uint32_t next = nexts[state][k];
                uint32_t new_state = ((state & 0x1F) << 5) | next; /* shift prev←cur, cur←next */
                ndp[new_state] += ways;
            }
        }
        /* Move ndp → dp. */
        for (size_t i = 0; i < 1024; ++i) dp[i] = ndp[i];
    }

    /* Final column must satisfy its constraint with left neighbor
       prev (contained in state) and right neighbor 0. */
    seq_t ans = 0;
    for (uint32_t state = 0; state < 1024; ++state) {
        if (dp[state] == 0) continue;
        uint32_t prev = state >> 5;
        uint32_t cur  = state & 0x1F;
        if (cur_is_valid(prev, cur, 0)) {
            ans += dp[state];
        }
    }
    return ans;
}
