/*
 * A296037  Number of nX6 0..1 arrays with each 1 adjacent to 2 or 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A296037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays where each 1 has exactly 2 or
 *            4 king‑neighbor 1s
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   check_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/check_transition.h"

/* forward declaration of helper for integer types */
static inline bool check_transition(uint8_t prevprev, uint8_t prev, uint8_t next);

/* transition table: bit k of transitions[p][q] is set iff mask k is a
   valid next row after rows (p,q). */
static uint64_t transitions[64][64];

/* Build the transition table once. */
static void init_transitions(void) {
    static bool inited = false;
    if (inited) return;
    for (uint8_t pp = 0; pp < 64; ++pp) {
        for (uint8_t pr = 0; pr < 64; ++pr) {
            uint64_t mask = 0;
            for (uint8_t nx = 0; nx < 64; ++nx) {
                if (check_transition(pp, pr, nx))
                    mask |= (1ULL << nx);
            }
            transitions[pp][pr] = mask;
        }
    }
    inited = true;
}

/* Main sequence function.  idx_t and seq_t are defined by the build. */
seq_t a(idx_t n) {
    init_transitions();

    static seq_t dp[64 * 64];
    static seq_t ndp[64 * 64];
    memset(dp, 0, sizeof(dp));

    /* First row: virtual row above is 0.  Each mask is a possible first row. */
    for (int m = 0; m < 64; ++m)
        dp[m] = 1;

    /* Process rows 2 .. n. */
    for (idx_t step = 2; step <= n; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (uint8_t pp = 0; pp < 64; ++pp) {
            int base_pp = pp * 64;
            for (uint8_t pr = 0; pr < 64; ++pr) {
                seq_t val = dp[base_pp + pr];
                if (!val) continue;
                uint64_t bm = transitions[pp][pr];
                while (bm) {
                    int nx = __builtin_ctzll(bm);
                    ndp[pr * 64 + nx] += val;
                    bm &= bm - 1;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Final flush with a virtual zero row at the bottom. */
    seq_t ans = 0;
    for (uint8_t pp = 0; pp < 64; ++pp) {
        int base_pp = pp * 64;
        for (uint8_t pr = 0; pr < 64; ++pr) {
            seq_t val = dp[base_pp + pr];
            if (!val) continue;
            if (transitions[pp][pr] & 1ULL)   /* bit 0 set => next row = 0 is allowed */
                ans += val;
        }
    }

    return ans;
}
