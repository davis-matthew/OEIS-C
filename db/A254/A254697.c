/*
 * A254697  Number of length n+4 0..7 arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 29, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming using a 4‑state automaton. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/* Transition table: next_state[s][d] = state after adding digit d to state s,
   or -1 if the 5‑window is invalid. */
static bool trans_done = false;
static short next_state[4096][8];

/* Encode a 4‑tuple of digits (0..7) into a state index 0..4095. */
static inline int encode_state(int a, int b, int c, int d)
{
    return (a << 9) | (b << 6) | (c << 3) | d;
}

/* Decode a state into its digits. */
static inline void decode_state(int state, int *a, int *b, int *c, int *d)
{
    *a = (state >> 9) & 7;
    *b = (state >> 6) & 7;
    *c = (state >> 3) & 7;
    *d = state & 7;
}

/* Test whether a 5‑tuple satisfies the window condition. */
static bool window_valid(const int v[5])
{
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int M = v[i] > v[j] ? v[i] : v[j];
            int m = 8;               /* larger than any digit 0..7 */
            for (int k = 0; k < 5; ++k) {
                if (k != i && k != j) {
                    if (v[k] < m) m = v[k];
                }
            }
            if (M == m) return true;
        }
    }
    return false;
}

/* Build the transition table once. */
static void build_transitions(void)
{
    if (trans_done) return;
    for (int s = 0; s < 4096; ++s) {
        int a, b, c, d;
        decode_state(s, &a, &b, &c, &d);
        for (int e = 0; e < 8; ++e) {
            int v[5] = {a, b, c, d, e};
            if (window_valid(v))
                next_state[s][e] = (short)encode_state(b, c, d, e);
            else
                next_state[s][e] = -1;
        }
    }
    trans_done = true;
}

/* Cache of already computed sequence values.  a_cache[n-1] = a(n). */
static seq_t *a_cache = NULL;
static idx_t a_cache_cap = 0;  /* capacity of a_cache */

static void ensure_cache(idx_t needed)
{
    if (needed <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache_cap = new_cap;
}

/* State machines for dynamic programming. */
static seq_t dp[4096];
static seq_t dp_next[4096];
static seq_t *dp_arr = dp;
static seq_t *next_arr = dp_next;
static idx_t steps_done = 0;   /* number of steps (n values) computed */

/* Main sequence function. */
seq_t a(idx_t n)
{
    assert(n >= 1);
    build_transitions();
    ensure_cache(n);          /* need slot 0..n-1 */

    /* Compute until we reach the requested n. */
    while (steps_done < n) {
        /* Initialise dp for the first step. */
        if (steps_done == 0) {
            for (int s = 0; s < 4096; ++s) dp_arr[s] = 1;
        }

        /* Clear next array. */
        memset(next_arr, 0, sizeof(dp_next));

        /* Transition. */
        for (int s = 0; s < 4096; ++s) {
            seq_t val = dp_arr[s];
            if (!val) continue;
            for (int d = 0; d < 8; ++d) {
                short ns = next_state[s][d];
                if (ns >= 0) {
                    next_arr[ns] += val;
                }
            }
        }

        /* Sum to get a(steps_done + 1). */
        seq_t sum = 0;
        for (int s = 0; s < 4096; ++s) sum += next_arr[s];

        /* Store in cache. */
        a_cache[steps_done] = sum;   /* steps_done == n-1 after this */

        /* Swap dp arrays for next iteration. */
        seq_t *tmp = dp_arr;
        dp_arr = next_arr;
        next_arr = tmp;

        ++steps_done;
    }

    return a_cache[n - 1];
}
