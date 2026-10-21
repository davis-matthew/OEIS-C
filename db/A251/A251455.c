/*
 * A251455  Number of (n+1)X(4+1) 0..3 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×5 arrays over {0,1,2,3} such that
 * every 2×2 subblock sums to a prime (OEIS A251455).
 * This implementation uses a row‑by‑row dynamic programming algorithm.
 */
 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* 0‑12: whether the sum is prime (2,3,5,7,11). */
static const bool prime_sum[13] = {
    false, /* 0 */
    false, /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    false, /* 6 */
    true,  /* 7 */
    false, /* 8 */
    false, /* 9 */
    false, /*10 */
    true,  /*11 */
    false  /*12 */
};

/* There are 2 bits per column → 10 bits for a 5‑column row → 1024 states. */
#define STATE_COUNT 1024

/* For each row r, compat[r] lists rows s that may follow r. */
static uint16_t compat[STATE_COUNT][STATE_COUNT];
static unsigned char compat_cnt[STATE_COUNT];
static bool compat_built = false;

/* Build the adjacency lists once. */
static void build_compat(void)
{
    for (int r = 0; r < STATE_COUNT; ++r) {
        int cnt = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int a = (r >> (2 * j)) & 3;
                int b = (r >> (2 * (j + 1))) & 3;
                int c = (s >> (2 * j)) & 3;
                int d = (s >> (2 * (j + 1))) & 3;
                int sum = a + b + c + d;
                if (!prime_sum[sum]) ok = false;
            }
            if (ok) {
                compat[r][cnt++] = (uint16_t)s;
            }
        }
        compat_cnt[r] = (unsigned char)cnt;
    }
    compat_built = true;
}

/* DP storage reused for each call. */
static seq_t dp[STATE_COUNT];
static seq_t next_row[STATE_COUNT];

seq_t a(idx_t n)
{
    /* Build adjacency lists lazily. */
    if (!compat_built) build_compat();

    /* First row: one way to start from any state. */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 1;

    /* Pointers to the current and next DP arrays. */
    seq_t *curr = dp;
    seq_t *next = next_row;

    /* Add n rows after the first: total height becomes n+1. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero the next array. */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        /* Transition from each current state to all compatible next states. */
        for (int r = 0; r < STATE_COUNT; ++r) {
            seq_t val = curr[r];
            if (val == 0) continue;
            unsigned char cnt = compat_cnt[r];
            uint16_t const *nb = compat[r];
            for (unsigned char k = 0; k < cnt; ++k) {
                next[nb[k]] += val;
            }
        }

        /* Swap pointers for next iteration. */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* Sum all counts in the final DP array. */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += curr[i];
    return total;
}
