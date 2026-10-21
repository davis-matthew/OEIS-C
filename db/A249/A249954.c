/*
 * A249954  Number of length n+5 0..2 arrays with no six consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of length n+5 arrays over {0,1,2} avoiding
 *            the pattern that in any 6 consecutive terms the maximum of some
 *            two terms equals the minimum of the remaining four terms. This
 *            is computed by a linear-time DP over the last five digits.
 *            Initial values: a(1)=285, a(2)=505, … (see OEIS)
 * Range:     u8 none, u16 n <= 10, u32 n <= 29, u64 n <= 67, u128 n <= 143,
 *            big any n
 * Helpers:   valid_window
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/valid_window.h"

#define STATE_COUNT 243            /* 3^5 possible last–five-digit states */
#define TRANS_INVALID 0xFFFFU

/* Transition table: trans[s][c] = next state when digit c is appended
   to the state s; TRANS_INVALID if the resulting window of 6 digits
   violates the forbidden pattern. */
static uint16_t trans[STATE_COUNT][3];
/* DP arrays for the current and next length */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
/* Cache of totals for each length L (L >= 5) */
static seq_t *totals = NULL;
static size_t totals_size = 0;
static idx_t cached_len = 0;   /* largest length that has been computed */

/* One–time initialization: compute transition table, allocate arrays,
   and set dp_cur for length = 5 (all states count 1). */
static void init(void)
{
    /* Precompute transitions */
    for (int id = 0; id < STATE_COUNT; ++id) {
        for (int c = 0; c < 3; ++c) {
            uint8_t tup[6];
            /* Decode the 5‑digit state into array 'tup[0..4]' */
            int tmp = id;
            for (int k = 4; k >= 0; --k) {
                tup[k] = tmp % 3;
                tmp /= 3;
            }
            tup[5] = (uint8_t)c; /* new digit */

            if (valid_window(tup))
                trans[id][c] = (uint16_t)(((unsigned)id % 81) * 3 + c);
            else
                trans[id][c] = TRANS_INVALID;
        }
    }

    /* Allocate DP arrays */
    dp_cur   = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    dp_next  = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    if (!dp_cur || !dp_next) exit(EXIT_FAILURE);

    /* Length 5: all 5‑digit sequences are allowed */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 1;

    /* Allocate totals array up to length 5 */
    totals = (seq_t *)malloc(sizeof(seq_t) * 6);
    if (!totals) exit(EXIT_FAILURE);
    totals_size = 6;
    totals[5] = 0; /* will be computed on first extend_to call */
    cached_len = 5;
}

/* Ensure totals array can hold up to new_max */
static void grow_totals(idx_t new_max)
{
    if ((size_t)new_max + 1 > totals_size) {
        size_t new_size = (size_t)new_max + 1;
        seq_t *new_totals = (seq_t *)realloc(totals, new_size * sizeof(seq_t));
        if (!new_totals) exit(EXIT_FAILURE);
        totals = new_totals;
        totals_size = new_size;
    }
}

/* Extend DP / totals up to need_len (inclusive) */
static void extend_to(idx_t need_len)
{
    if (cached_len >= need_len) return;
    if (!dp_cur) init();   /* first time */

    grow_totals(need_len);

    /* Compute totals[5] (number of length‑5 arrays) */
    if (cached_len < 5) {
        seq_t sum5 = 0;
        for (int i = 0; i < STATE_COUNT; ++i) sum5 += dp_cur[i];
        totals[5] = sum5;
    }

    /* DP for lengths from 6 up to need_len */
    for (idx_t L = cached_len + 1; L <= need_len; ++L) {
        /* zero dp_next */
        memset(dp_next, 0, sizeof(seq_t) * STATE_COUNT);

        /* transitions */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            uint16_t trow0 = trans[s][0];
            uint16_t trow1 = trans[s][1];
            uint16_t trow2 = trans[s][2];
            if (trow0 != TRANS_INVALID) dp_next[trow0] += val;
            if (trow1 != TRANS_INVALID) dp_next[trow1] += val;
            if (trow2 != TRANS_INVALID) dp_next[trow2] += val;
        }

        /* swap dp arrays */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;

        /* compute total for this length */
        seq_t total = 0;
        for (int i = 0; i < STATE_COUNT; ++i) total += dp_cur[i];
        totals[(idx_t)L] = total;
    }

    cached_len = need_len;
}

/* Public API required by the test harness */
seq_t a(idx_t n)
{
    /* n >= 1 -> length = n + 5 */
    idx_t L = n + 5;
    if (L > cached_len) extend_to(L);
    return totals[(idx_t)L];
}
