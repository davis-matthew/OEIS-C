/*
 * A259521  Number of (n+1)X(5+1) 0..1 arrays with each 2X2 subblock having
 *          clockwise pattern 0000 0001 0011 0101 or 1111.
 * https://oeis.org/A259521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259521: number of (n+1)×6 0..1 arrays whose every 2×2 subblock is a cyclic
   rotation of one of 0000, 0001, 0011, 0101 or 1111.  Computed by an O(n·64²) transfer‑matrix
   dynamic programme. */

#include <stdbool.h>

#define STATES 64

/* For each row pattern s (0..63), nexts[s] holds all t that are compatible with s.
   next_count[s] is the number of stored next states. */
static uint8_t nexts[STATES][STATES];
static uint8_t next_count[STATES];

/* Build the transition table once. */
static void init_transition(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    for (int s = 0; s < STATES; ++s) {
        next_count[s] = 0;
        for (int t = 0; t < STATES; ++t) {
            bool ok = true;
            for (int c = 0; c < 5; ++c) {
                uint8_t top1  = (s >> c)      & 1U;
                uint8_t top2  = (s >> (c + 1)) & 1U;
                uint8_t bot1  = (t >> c)      & 1U;
                uint8_t bot2  = (t >> (c + 1)) & 1U;
                /* 2×2 block as bits a,b,c,d in that order. */
                uint8_t p = (top1 << 3) | (top2 << 2) | (bot1 << 1) | bot2;
                /* Allowed patterns: {0000,0001,0010,0100,1000,0011,0110,1100,1001,0101,1010,1111}. */
                if (!(p == 0x0 || p == 0x1 || p == 0x2 || p == 0x4 || p == 0x8 ||
                      p == 0x3 || p == 0x6 || p == 0xC || p == 0x9 ||
                      p == 0x5 || p == 0xA || p == 0xF)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                nexts[s][next_count[s]++] = (uint8_t)t;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_transition();

    /* Current count of ways to reach each row pattern after the last processed row. */
    seq_t vec[STATES];
    seq_t new_vec[STATES];

    /* All first rows are equally possible: one way for each of the 64 patterns. */
    for (int i = 0; i < STATES; ++i) vec[i] = 1;

    /* We need exactly n transitions to reach the (n+1)-st row. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) new_vec[i] = 0;
        for (int s = 0; s < STATES; ++s) {
            seq_t v = vec[s];
            if (v == 0) continue;
            const uint8_t *ns = nexts[s];
            uint8_t cnt = next_count[s];
            for (uint8_t k = 0; k < cnt; ++k) {
                new_vec[ns[k]] += v;
            }
        }
        /* Move to next iteration. */
        for (int i = 0; i < STATES; ++i) vec[i] = new_vec[i];
    }

    /* Sum over all possible last rows. */
    seq_t ans = 0;
    for (int i = 0; i < STATES; ++i) ans += vec[i];
    return ans;
}
