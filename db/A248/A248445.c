/*
 * A248445  Number of length n+5 0..5 arrays with no three disjoint pairs in
 *          any consecutive six terms having the same sum.
 * https://oeis.org/A248445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 20, u128 n <= 47,
 *            big any n
 * Helpers:   has_equal_sum_matching, populate_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248445: Number of length n+5 0..5 arrays with no three disjoint pairs
 * in any consecutive six terms having the same sum.
 *
 * The implementation uses dynamic programming over the last five digits
 * (state space 6^5 = 7776).  For each state (a,b,c,d,e) the transition
 * appends a new digit v, checks that the 6‑term window {a,b,c,d,e,v}
 * cannot be perfectly matched into three pairs with equal sums, and
 * accumulates counts.  The check is performed using frequencies of the
 * six values (0..5). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_equal_sum_matching.h"
#include "../utils/populate_counts.h"

#define STATE_COUNT 7776

/* Transition table: next_state[s][v] gives the state after appending v. */
static unsigned short next_state[STATE_COUNT][6];
/* For each state, a bitmask of digits v that keep the 6‑term window valid. */
static unsigned char valid_mask[STATE_COUNT];
static bool initialized = false;

/* Helper: retrieve individual digits from a state index. */
static void
decode_state(unsigned short idx, unsigned char *a, unsigned char *b,
             unsigned char *c, unsigned char *d, unsigned char *e)
{
    *a = (idx / 1296) % 6;      /* 6^4 = 1296 */
    *b = (idx / 216) % 6;       /* 6^3 = 216  */
    *c = (idx / 36) % 6;        /* 6^2 = 36   */
    *d = (idx / 6) % 6;         /* 6^1 = 6    */
    *e = idx % 6;               /* 6^0 = 1    */
}

/* Lazy initialization of transition tables. */
static void
init_tables(void)
{
    if (initialized) return;
    for (unsigned short idx = 0; idx < STATE_COUNT; ++idx) {
        unsigned char a, b, c, d, e;
        decode_state(idx, &a, &b, &c, &d, &e);
        for (unsigned char v = 0; v < 6; ++v) {
            /* New state: drop a, keep b,c,d,e, add v */
            unsigned short nxt = (unsigned short)(b * 1296 + c * 216 + d * 36 + e * 6 + v);
            next_state[idx][v] = nxt;

            /* Build counts of the 6‑term window */
            unsigned char counts[6];
            populate_counts(counts, a, b, c, d, e, v);

            /* Window is forbidden if a matching exists */
            if (!has_equal_sum_matching(counts))
                valid_mask[idx] |= (1 << v);
        }
    }
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n >= 1; n = 0 safe but unused */
    init_tables();

    static seq_t dp_cur[STATE_COUNT];
    static seq_t dp_nxt[STATE_COUNT];

    /* Initialize all 5‑digit prefixes as allowed */
    for (unsigned short s = 0; s < STATE_COUNT; ++s)
        dp_cur[s] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        /* zero dp_nxt */
        for (unsigned short s = 0; s < STATE_COUNT; ++s)
            dp_nxt[s] = 0;
        for (unsigned short s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            unsigned char mask = valid_mask[s];
            while (mask) {
                unsigned char bit = __builtin_ctz((unsigned int)mask) & 63;
                mask &= mask - 1;
                unsigned short ns = next_state[s][bit];
                dp_nxt[ns] += val;
            }
        }
        /* swap dp_cur and dp_nxt */
        for (unsigned short s = 0; s < STATE_COUNT; ++s)
            dp_cur[s] = dp_nxt[s];
    }

    /* Sum over all final states */
    seq_t total = 0;
    for (unsigned short s = 0; s < STATE_COUNT; ++s)
        total += dp_cur[s];
    return total;
}
