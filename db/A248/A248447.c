/*
 * A248447  Number of length n+5 0..7 arrays with no three disjoint pairs in
 *          any consecutive six terms having the same sum.
 * https://oeis.org/A248447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 16, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Implements A248447 by dynamic programming over 5‑tuples of digits.
   The state is a tuple (a,b,c,d,e) of the last five digits; for each
   new digit f we test all 15 ways to split the six‑tuple (a,b,c,d,e,f)
   into three disjoint pairs.  If any splitting yields equal sums for
   all three pairs, the transition is forbidden.  The DP counts all
   length‑(n+5) 0..7 arrays that avoid the forbidding condition. */

#define NUM_STATES 32768          /* 8^5 possible 5‑tuples */
#define NUM_ALPH   8              /* digits 0..7 */
#define MASK15    0x7FFF          /* keep 15 low bits for state shift */

/* Transition validity table: 1 = allowed, 0 = forbidden */
static uint8_t valid_trans[NUM_STATES * NUM_ALPH];
/* Resulting next state when appending a digit */
static uint16_t next_state[NUM_STATES * NUM_ALPH];

/* All 15 matchings of indices 0..5 into 3 disjoint pairs (15 × 3 × 2) */
static const int partition[15][3][2] = {
    {{0,1},{2,3},{4,5}},
    {{0,1},{2,4},{3,5}},
    {{0,1},{2,5},{3,4}},
    {{0,2},{1,3},{4,5}},
    {{0,2},{1,4},{3,5}},
    {{0,2},{1,5},{3,4}},
    {{0,3},{1,2},{4,5}},
    {{0,3},{1,4},{2,5}},
    {{0,3},{1,5},{2,4}},
    {{0,4},{1,2},{3,5}},
    {{0,4},{1,3},{2,5}},
    {{0,4},{1,5},{2,3}},
    {{0,5},{1,2},{3,4}},
    {{0,5},{1,3},{2,4}},
    {{0,5},{1,4},{2,3}}
};

static bool tables_initialized = false;
static void init_tables(void)
{
    if (tables_initialized) return;
    for (int s = 0; s < NUM_STATES; ++s) {
        /* Decode the 5 digits of state s */
        int d[5];
        d[0] = (s >> 12) & 7;
        d[1] = (s >> 9)  & 7;
        d[2] = (s >> 6)  & 7;
        d[3] = (s >> 3)  & 7;
        d[4] =  s        & 7;

        /* Precompute transitions for all next digits */
        int base = (s << 3) & MASK15;   /* drop oldest digit */
        for (int v = 0; v < NUM_ALPH; ++v) {
            int idx = s * NUM_ALPH + v;
            next_state[idx] = (uint16_t)(base | v);

            /* Form the six‑tuple (a,b,c,d,e,v) */
            int t[6] = {d[0], d[1], d[2], d[3], d[4], v};
            bool ok = true;
            for (int p = 0; p < 15 && ok; ++p) {
                int s0 = t[partition[p][0][0]] + t[partition[p][0][1]];
                int s1 = t[partition[p][1][0]] + t[partition[p][1][1]];
                int s2 = t[partition[p][2][0]] + t[partition[p][2][1]];
                if (s0 == s1 && s1 == s2) ok = false;
            }
            valid_trans[idx] = ok ? 1 : 0;
        }
    }
    tables_initialized = true;
}

/* DP storage for two consecutive layers */
static seq_t dp0[NUM_STATES];
static seq_t dp1[NUM_STATES];
static seq_t *dp_cur  = dp0;
static seq_t *dp_next = dp1;
static idx_t  cur_len = 5;          /* length represented by dp_cur */
static bool   dp_inited = false;

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of offset; undefined */
    idx_t target_len = n + 5;        /* total length of the array */

    init_tables();
    if (!dp_inited) {
        for (int i = 0; i < NUM_STATES; ++i) dp_cur[i] = 1; /* all 5‑tuples allowed */
        cur_len = 5;
        dp_inited = true;
    }

    /* Extend DP to the required length */
    while (cur_len < target_len) {
        for (int i = 0; i < NUM_STATES; ++i) dp_next[i] = 0;
        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            int base = s * NUM_ALPH;
            for (int v = 0; v < NUM_ALPH; ++v) {
                if (valid_trans[base + v]) {
                    int dst = next_state[base + v];
                    dp_next[dst] += val;
                }
            }
        }
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        ++cur_len;
    }

    /* Sum all states to get the final count */
    seq_t total = 0;
    for (int i = 0; i < NUM_STATES; ++i) total += dp_cur[i];
    return total;
}
