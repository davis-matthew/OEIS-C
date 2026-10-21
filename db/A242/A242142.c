/*
 * A242142  Number of length n+5 0..6 arrays with no consecutive six elements
 *          summing to more than 3*6.
 * https://oeis.org/A242142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of length n+5 arrays of digits 0..6 in
 *            which every consecutive 6 digits sum to at most 18. A_242142
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 19, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ------- Global tables (fixed maximum of 7^5 = 16807 states) ------- */
#define MAX_STATES 16807
static int16_t   oldToNew[MAX_STATES];   /* mapping from base‑7 id to compact state idx  */
static uint8_t   sum5      [MAX_STATES]; /* sum of the 5 digits of that tuple          */
static uint16_t  nxt       [MAX_STATES][7]; /* for each state, list of next state idx      */
static uint8_t   deg       [MAX_STATES]; /* number of outgoing transitions per state   */
static seq_t    dp_cur   [MAX_STATES]; /* counts for current length                  */
static seq_t    dp_next  [MAX_STATES]; /* temporary buffer for next length           */
static seq_t    cache    [20000];      /* cached a(n) values  (a(1)..a(20000))      */
static size_t   cache_len = 0;         /* how many a(n) values currently cached     */

static size_t   stateCnt   = 0;        /* number of valid states (sum <=18)          */
static bool     init_done  = false;    /* whether tables have been initialized       */

/* --------------------------------------------------------------- */
/* Build state machine, transition table, and initial DP vector   */
/* --------------------------------------------------------------- */
static void init_tables(void)
{
    const int POW7_4 = 2401; /* 7^4 */

    /* Step 1: enumerate all 5‑tuples, keeping those with sum ≤ 18 */
    int idx = 0;
    for (int id = 0; id < MAX_STATES; ++id) {
        int t = id, s = 0;
        for (int k = 0; k < 5; ++k) {
            s += t % 7;
            t /= 7;
        }
        if (s <= 18) {
            oldToNew[id] = (int16_t)idx;
            sum5[id]  = (uint8_t)s;
            ++idx;
        } else {
            oldToNew[id] = -1;            /* sentinel for illegal tuple   */
        }
    }
    stateCnt = (size_t)idx;

    /* Step 2: build transition table */
    for (int id = 0; id < MAX_STATES; ++id) {
        int curIdx = oldToNew[id];
        if (curIdx < 0) continue;           /* skip invalid state          */
        uint8_t sum = sum5[id];
        uint8_t d = 0;
        for (int v = 0; v < 7; ++v) {
            if (sum + v <= 18) {
                int newOld = (id % POW7_4) * 7 + v;      /* shift */
                int newIdx = oldToNew[newOld];
                if (newIdx >= 0) {
                    nxt[curIdx][d++] = (uint16_t)newIdx;
                }
            }
        }
        deg[curIdx] = d;
    }

    /* Step 3: initialise DP for length 5 (each valid 5‑tuple appears once) */
    for (size_t i = 0; i < stateCnt; ++i) dp_cur[i] = (seq_t)1;

    /* No a(n) values cached yet */
    cache_len  = 0;
    init_done  = true;
}

/* --------------------------------------------------------------- */
/* Ensure that cache[n‑1] (i.e., a(n)) is computed for all n ≤ target */
/* --------------------------------------------------------------- */
static void ensure_upto(idx_t n)
{
    if (!init_done) init_tables();

    /* extend cache until we have a(n) for this n */
    while (cache_len < (size_t)n) {
        /* zero dp_next */
        for (size_t i = 0; i < stateCnt; ++i) dp_next[i] = (seq_t)0;

        /* transition from current to next length */
        for (size_t i = 0; i < stateCnt; ++i) {
            seq_t val = dp_cur[i];
            if (!val) continue;
            uint8_t d = deg[i];
            for (uint8_t k = 0; k < d; ++k) {
                int j = nxt[i][k];
                dp_next[j] += val;
            }
        }

        /* compute a(length) = sum of all states in dp_next */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < stateCnt; ++i) total += dp_next[i];
        cache[cache_len++] = total;

        /* prepare for next iteration: copy dp_next to dp_cur */
        for (size_t i = 0; i < stateCnt; ++i) dp_cur[i] = dp_next[i];
    }
}

/* --------------------------------------------------------------- */
/* Public function required by the build                                    */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_upto(n);
    return cache[n - 1];
}
