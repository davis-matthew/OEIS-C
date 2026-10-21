/*
 * A249082  Number of length n+5 0..5 arrays with every six consecutive terms
 *          having two times the sum of some two elements equal to the sum of
 *          the remaining four.
 * https://oeis.org/A249082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 19, u64 n <= 52, u128 n <= 118,
 *            big any n
 * Helpers:   is_valid_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A249082
 *
 *  a(n) = number of length n+5 arrays with entries in {0,1,2,3,4,5}
 *  such that every consecutive block of 6 elements contains a pair of
 *  elements whose sum multiplied by 3 equals the sum of the other four
 *  elements.  The block condition is equivalent to: for a 6‑tuple
 *  (x0,…,x5) there exist i<j with 3·(xi+xj)=sum(x0,…,x5).
 *
 *  The function computes a(n) by a sliding‑window DP.
 *
 *  Complexity: O(n·6^5) time, O(6^5) memory.  For n=10 000 this is
 *  about 4.7⋅10^8 elementary operations, well under the limits for
 *  64‑bit integers and modest for arbitrary precision arithmetic.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_valid_block.h"

#define STATE_CNT 7776          /* 6^5 = 7776 */
#define NEXT_SENTINEL 0xFFFFU   /* invalid next state marker */

/* Precompute the transition table once */
static unsigned short next_state[STATE_CNT][6];
static bool initialized = false;

static void precompute_transitions(void) {
    int pow6[5] = {7776, 1296, 216, 36, 6};
    unsigned char digits[5];
    for (int s = 0; s < STATE_CNT; ++s) {
        int t = s;
        for (int k = 4; k >= 0; --k) {
            digits[k] = t % 6;
            t /= 6;
        }
        for (int x = 0; x < 6; ++x) {
            unsigned char block[6];
            for (int k = 0; k < 5; ++k) block[k] = digits[k];
            block[5] = (unsigned char)x;
            if (!is_valid_block(block)) {
                next_state[s][x] = NEXT_SENTINEL;
                continue;
            }
            /* new state: drop the oldest digit and add x */
            int new_s = (s % 1296) * 6 + x;   /* s % 6^4 = s % 1296 */
            next_state[s][x] = (unsigned short)new_s;
        }
    }
    initialized = true;
}

/******************************************************************
 *  Main function required by the benchmark framework.
 ******************************************************************/

seq_t a(idx_t n) {
    /* number of steps equals n (array length = n+5) */
    if (!initialized) precompute_transitions();

    /* DP arrays */
    static seq_t dp[STATE_CNT];
    static seq_t ndp[STATE_CNT];
    seq_t *cur = dp;
    seq_t *next = ndp;

    /* initial state: all 5‑digit prefixes count 1 */
    size_t i;
    for (i = 0; i < STATE_CNT; ++i) cur[i] = 1;

    /* iterate n times, each time appending one more digit */
    for (i = 0; i < n; ++i) {
        memset(next, 0, sizeof(ndp));
        for (size_t s = 0; s < STATE_CNT; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            unsigned short ns;
            for (int x = 0; x < 6; ++x) {
                ns = next_state[s][x];
                if (ns != NEXT_SENTINEL) {
                    next[ns] += val;
                }
            }
        }
        /* swap arrays for the next step */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum all counts for the final length n+5 */
    seq_t res = 0;
    for (i = 0; i < STATE_CNT; ++i) res += cur[i];
    return res;
}
