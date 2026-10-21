/*
 * A248067  Number of length n+5 0..7 arrays with some disjoint triples in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 28, u128 n <= 64,
 *            big any n
 * Helpers:   window_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A248067: Number of length n+5 0..7 arrays where each
    consecutive six terms contain a pair of disjoint triples with equal sum.
    The function a(n) is computed by a dynamic‑programming recurrence
    that stores the number of valid arrays of each length. */
/*  The implementation uses a 5‑digit state (last 5 elements) and
    precomputes, for each state, the next possible digits that keep the
    property for the new 6‑window. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/window_ok.h"

#define STATE_COUNT 32768       /* 8^5 */

/* transition data for each state */
static unsigned char next_cnt[STATE_COUNT];
static uint32_t next_tab[STATE_COUNT][8];

/* DP arrays */
static seq_t *curr  = NULL;
static seq_t *next_arr = NULL;

/* memoisation buffer */
static seq_t *memo = NULL;
static size_t memo_len = 0;
static size_t memo_cap = 0;

/* initialise transition tables and the first term (length 6) */
static void init_tables(void)
{
    if (curr) return;                   /* already initialised */

    curr = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    next_arr = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    if (!curr || !next_arr) exit(1);

    /* clear DP arrays */
    for (size_t i = 0; i < STATE_COUNT; ++i) curr[i] = 0;

    /* build transition table: for each state, store allowed next digits */
    for (size_t s = 0; s < STATE_COUNT; ++s) {
        unsigned char digits[5];
        digits[0] = (unsigned char)(s & 7);
        digits[1] = (unsigned char)((s >> 3) & 7);
        digits[2] = (unsigned char)((s >> 6) & 7);
        digits[3] = (unsigned char)((s >> 9) & 7);
        digits[4] = (unsigned char)((s >> 12) & 7);

        unsigned char count = 0;
        for (int x = 0; x < 8; ++x) {
            unsigned char t[6];
            t[0] = digits[0]; t[1] = digits[1];
            t[2] = digits[2]; t[3] = digits[3];
            t[4] = digits[4]; t[5] = (unsigned char)x;
            if (window_ok(t)) {
                next_tab[s][count] = (uint32_t)((s >> 3) | ((unsigned int)x << 12));
                count++;
            }
        }
        next_cnt[s] = count;
    }

    /* count length‑6 arrays: for each valid 6‑tuple, increment the state counter */
    unsigned char a,b,c,d,e,f;
    for (a = 0; a < 8; ++a)
    for (b = 0; b < 8; ++b)
    for (c = 0; c < 8; ++c)
    for (d = 0; d < 8; ++d)
    for (e = 0; e < 8; ++e)
    for (f = 0; f < 8; ++f) {
        unsigned char t[6] = {a,b,c,d,e,f};
        if (!window_ok(t)) continue;
        size_t s = b + (c << 3) + (d << 6) + (e << 9) + (f << 12);
        curr[s] += (seq_t)1;
    }

    /* store a(1) */
    seq_t total = 0;
    for (size_t i = 0; i < STATE_COUNT; ++i) total += curr[i];

    memo = (seq_t *)malloc(sizeof(seq_t));
    if (!memo) exit(1);
    memo[0] = total;
    memo_len = 1;
    memo_cap = 1;
}

/* extend memoisation up to the nth term (n >= 2) */
static void extend_to(size_t n)
{
    while (memo_len < n) {
        /* reset next array */
        for (size_t i = 0; i < STATE_COUNT; ++i) next_arr[i] = 0;

        seq_t step_total = 0;
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = curr[s];
            if (!val) continue;
            unsigned char kcnt = next_cnt[s];
            for (unsigned char k = 0; k < kcnt; ++k) {
                size_t ns = next_tab[s][k];
                next_arr[ns] += val;
                step_total += val;
            }
        }

        /* swap arrays for next iteration */
        seq_t *tmp = curr; curr = next_arr; next_arr = tmp;

        /* grow memo buffer if needed */
        if (memo_len >= memo_cap) {
            size_t new_cap = memo_cap ? memo_cap * 2 : 16;
            seq_t *newmem = (seq_t *)realloc(memo, sizeof(seq_t) * new_cap);
            if (!newmem) exit(1);
            memo = newmem;
            memo_cap = new_cap;
        }

        memo[memo_len++] = step_total;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_tables();
    size_t sz = (size_t)n;
    if (memo_len < sz) extend_to(sz);
    return memo[sz - 1];
}
