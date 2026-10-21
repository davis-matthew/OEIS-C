/*
 * A241935  Number of length n+4 0..7 arrays with no consecutive five elements
 *          summing to more than 2*7.
 * https://oeis.org/A241935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of length n+4 over {0,…,7} with * no
 *            consecutive five elements summing to more than 2*7. * Computed
 *            by dynamic programming over the last four elements * (4096
 *            states). For each state the allowed next element x is * x ≤ 14 -
 *            (a+b+c+d). The DP works in O(n*4096) time
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
# include "../utils/seq_alloc.h"     /* seq_alloc / seq_realloc for big ints */
# include "../utils/seq_realloc.h"
#endif

/* number of bits to encode 4 values 0..7 */
#define STATE_BITS   12
#define STATE_COUNT (1U << STATE_BITS)   /* 4096 states */

/* Global DP state */
static seq_t *dp_prev = NULL;          /* counts for length L-1 */
static seq_t *dp_cur  = NULL;          /* counts for length L   */

/* Mapping from length to total counts.
 * Index L stores number of arrays of length L (L ≥ 4).
 * len_counts_cap is the allocated capacity. */
static seq_t *len_counts = NULL;
static size_t len_counts_cap = 4;      /* indices 0..3 initially unused */

/* Highest length that has been computed so far.  We start at 3 so that
 * the first iteration handles length 4 (the minimum array length). */
static idx_t max_len_computed = 3;

/* Transition tables: for every state the list of next states
 * reachable by adding a digit x with (state_sum + x) ≤ 14. */
static unsigned char sum4[STATE_COUNT];
static unsigned char cnt_next[STATE_COUNT];
static idx_t next_state[STATE_COUNT][8];

/* ----------------------------------------------------------------- */
/* Encode four 3‑bit digits into a 12‑bit state. */
static inline idx_t
encode4(unsigned a, unsigned b, unsigned c, unsigned d)
{
    return ((idx_t)a << 9) | ((idx_t)b << 6) | ((idx_t)c << 3) | (idx_t)d;
}

/* Decode a state into four digits. */
static inline void
decode4(idx_t s, unsigned *a, unsigned *b, unsigned *c, unsigned *d)
{
    *a = (s >> 9) & 7u;
    *b = (s >> 6) & 7u;
    *c = (s >> 3) & 7u;
    *d =  s        & 7u;
}

/* ----------------------------------------------------------------- */
/* Prepare transition tables – called once. */
static void
init_transitions(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    for (idx_t s = 0; s < STATE_COUNT; ++s) {
        unsigned a, b, c, d;
        decode4(s, &a, &b, &c, &d);
        sum4[s] = a + b + c + d;
    }

    for (idx_t s = 0; s < STATE_COUNT; ++s) {
        unsigned char *cnt = &cnt_next[s];
        *cnt = 0;
        unsigned char sum = sum4[s];
        for (unsigned x = 0; x < 8u; ++x) {
            if (sum + x <= 14u) {                /* allowed next digit */
                unsigned a, b, c, d;
                decode4(s, &a, &b, &c, &d);
                next_state[s][(*cnt)++] = encode4(b, c, d, x);
            }
        }
    }
}

/* ----------------------------------------------------------------- */
/* Allocate DP arrays once.  dp_prev is initialized with 1 for all
 * states (length 4).  No reallocation after that is needed. */
static void
init_dp_arrays(void)
{
    if (dp_prev) return;       /* already initialised */

#ifdef __cplusplus
    dp_prev = seq_alloc(STATE_COUNT);
    dp_cur  = seq_alloc(STATE_COUNT);
#else
    dp_prev = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    dp_cur  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
#endif
    for (idx_t s = 0; s < STATE_COUNT; ++s) dp_prev[s] = 1;   /* length 4 */
}

/* ----------------------------------------------------------------- */
/* Allocate/resize the len_counts array to hold counts up to length L. */
static void
ensure_len_counts_cap(idx_t L)
{
    if (len_counts == NULL) {
        /* first allocation: capacity at least L+1 */
        size_t cap = (L + 1 > len_counts_cap) ? (L + 1) : len_counts_cap;
        if (cap < 4) cap = 4;   /* minimal capacity for indices 0..3 */
#ifdef __cplusplus
        len_counts = seq_alloc(cap);
#else
        len_counts = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
        for (size_t i = 0; i < cap; ++i) len_counts[i] = 0;
        len_counts_cap = cap;
    } else if ((idx_t)len_counts_cap <= L) {
        /* enlarge */
        size_t new_cap = L + 1;
#ifdef __cplusplus
        len_counts = seq_realloc(len_counts, len_counts_cap, new_cap);
#else
        len_counts = (seq_t *)realloc(len_counts, new_cap * sizeof(seq_t));
#endif
        for (size_t i = len_counts_cap; i < new_cap; ++i) len_counts[i] = 0;
        len_counts_cap = new_cap;
    }
}

/* ----------------------------------------------------------------- */
/* Compute counts up to the desired length target_len (≥4). */
static void
ensure_computed_upto(idx_t target_len)
{
    if (target_len <= max_len_computed) return;

    init_transitions();
    init_dp_arrays();
    ensure_len_counts_cap(target_len);

    for (idx_t len = max_len_computed + 1; len <= target_len; ++len) {
        if (len == 4) {
            /* base case – each of the 4096 states occurs once */
            seq_t total = 0;
            for (idx_t s = 0; s < STATE_COUNT; ++s) total += dp_prev[s];
            len_counts[len] = total;
            /* dp_prev already contains the correct counts for length 4 */
            continue;
        }

        /* zero the next layer */
        for (idx_t s = 0; s < STATE_COUNT; ++s) dp_cur[s] = 0;

        /* transition from dp_prev to dp_cur */
        for (idx_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_prev[s];
            if (!val) continue;
            unsigned char cnt = cnt_next[s];
            for (unsigned t = 0; t < cnt; ++t) {
                idx_t ns = next_state[s][t];
                dp_cur[ns] += val;
            }
        }

        /* sum of dp_cur gives total arrays of this length */
        seq_t total = 0;
        for (idx_t s = 0; s < STATE_COUNT; ++s) total += dp_cur[s];
        len_counts[len] = total;

        /* prepare for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    max_len_computed = target_len;
}

/* ----------------------------------------------------------------- */
/* Public entry point.  Returns a(n) = number of arrays of length n+4. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; a(0) unused */
    idx_t target_len = n + 4;          /* length of arrays to count */
    ensure_computed_upto(target_len);
    return len_counts[target_len];
}
