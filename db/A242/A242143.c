/*
 * A242143  Number of length n+5 0..7 arrays with no consecutive six elements
 *          summing to more than 3*7.
 * https://oeis.org/A242143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Counts of length n+5 arrays over {0,…,7} whose every *
 *            consecutive six entries sum to at most 21. * Computed by dynamic
 *            programming over the last five elements
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 18, u128 n <= 41,
 *            big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

#define STATE_BITS 15          /* 5 digits × 3 bits */
#define STATES (1 << STATE_BITS)   /* 8⁵ = 32768 */

/* Pre‑computed data for each state (5–tuple). */
static uint8_t  sum5[STATES];
static uint16_t next_state[STATES][8];
static int initialized = 0;

static void
init_state_tables(void)
{
    if (initialized)
        return;
    initialized = 1;
    for (uint32_t st = 0; st < STATES; ++st) {
        uint8_t a1 = (st >> 12) & 7;
        uint8_t a2 = (st >> 9) & 7;
        uint8_t a3 = (st >> 6) & 7;
        uint8_t a4 = (st >> 3) & 7;
        uint8_t a5 = st & 7;
        sum5[st] = a1 + a2 + a3 + a4 + a5;

        uint32_t base = st & 0xFFF;   /* lower 12 bits = a2..a5 */
        for (int x = 0; x < 8; ++x)
            next_state[st][x] = (uint16_t)((base << 3) | x);
    }
}

/* DP tables for current and next step. */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;

/* Memoisation of a(n). */
static seq_t *memo   = NULL;
static size_t memo_cap = 0;
static idx_t  memo_len = 0;   /* highest n computed */

/* ------------------------------------------------------------------ */

seq_t
a(idx_t n)
{
    init_state_tables();

    if (n == 0) {
        /* Offset 1: a(1) is first term. a(0) is unused. */
        seq_t zero = 0;
        return zero;
    }

    /* Resize memo array if necessary. */
    if (n + 1 > memo_cap) {
        size_t new_cap = n + 1;
        seq_t *new_mem = seq_realloc(memo, memo_cap, new_cap);
        if (!new_mem)      /* allocation failure – return 0 as fallback */
            return (seq_t)0;
        memo = new_mem;
        memo_cap = new_cap;
    }

    /* Allocate DP arrays on first call. */
    if (!dp_cur) {
        dp_cur  = alloc_seq_array(STATES);
        dp_next = alloc_seq_array(STATES);
        /* Initial state: all 5‑tuples are possible. */
        for (uint32_t i = 0; i < STATES; ++i)
            dp_cur[i] = (seq_t)1;
    }

    /* Compute momentum up to the requested index. */
    while (memo_len < n) {
        idx_t step = memo_len + 1;   /* next term to compute */

        /* Clear next table. */
        for (uint32_t i = 0; i < STATES; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition step. */
        for (uint32_t s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0)
                continue;
            uint8_t sum = sum5[s];
            int maxx = 21 - sum;
            if (maxx > 7)
                maxx = 7;
            if (maxx < 0)
                continue;
            for (int x = 0; x <= maxx; ++x) {
                uint16_t ns = next_state[s][x];
                dp_next[ns] += val;
            }
        }

        /* Sum all reachable states: this is a(step). */
        seq_t total = (seq_t)0;
        for (uint32_t i = 0; i < STATES; ++i)
            total += dp_next[i];
        memo[step] = total;

        /* Prepare for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        memo_len = step;
    }

    return memo[n];
}
