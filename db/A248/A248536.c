/*
 * A248536  Number of length n+3 0..7 arrays with every four consecutive terms
 *          having the sum of some three elements equal to three times the
 *          fourth.
 * https://oeis.org/A248536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 37, u64 n <= 85, u128 n <= 182,
 *            big any n
 * Helpers:   good_quad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A248536:
 * Number of length n+3 0..7 arrays with every four consecutive terms having the sum of some three elements equal to three times the fourth.
 * The algorithm uses dynamic programming over the last three elements (states of size 8³ = 512).
 * For each state (i,j,k) all possible next elements d∈{0…7} that satisfy the 4‑term condition are pre‑computed.
 * Counts for sequences of length L (L = n+3) are obtained by iterating the transition matrix.
 * The sequence values are stored in a static cache so arbitrary order of calls is supported.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/good_quad.h"

typedef uint64_t idx_t;   /* Assume idx_t is defined by the build */

seq_t a(idx_t n);

static seq_t *dp = NULL;          /* DP array for current length L */
static seq_t *nextdp = NULL;     /* DP array for length L+1 */
static idx_t current_length = 3; /* Length corresponding to dp (>=3) */

static seq_t *a_cache = NULL;    /* Cache of computed terms a(1)..a(max) */
static size_t a_cache_cap = 0;   /* Allocated size of a_cache (>= max+1) */
static size_t a_cache_used = 0;  /* Highest n computed so far (>=0) */

static uint16_t trans_succ[512][8]; /* Successor states for each state */
static uint8_t trans_count[512];    /* Number of successors for each state */
static bool trans_init = false;     /* Transition table initialised */

/* Initialise the transition table once. */
static void init_transitions(void)
{
    if (trans_init) return;
    for (int s = 0; s < 512; ++s) {
        int i = s >> 6;          /* /64 */
        int j = (s >> 3) & 7;    /* ( /8 ) % 8 */
        int k = s & 7;           /* %8 */
        uint8_t cnt = 0;
        for (int d = 0; d < 8; ++d) {
            if (good_quad(i, j, k, d)) {
                int ns = (j << 6) | (k << 3) | d; /* j*64 + k*8 + d */
                trans_succ[s][cnt++] = (uint16_t)ns;
            }
        }
        trans_count[s] = cnt;
    }
    trans_init = true;
}

/* Initialise dp arrays */
static void init_dp_arrays(void)
{
    if (dp) return;
    dp = (seq_t *)malloc(512 * sizeof(seq_t));
    nextdp = (seq_t *)malloc(512 * sizeof(seq_t));
    for (size_t i = 0; i < 512; ++i) dp[i] = 1; /* length 3: all states possible */
}

/* Ensure a_cache has space for at least n+1 elements */
static void ensure_a_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= a_cache_cap) return;
    size_t new_cap = need * 2;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* Allocation failed – abort (should not happen) */
        abort();
    }
    a_cache = new_cache;
    a_cache_cap = new_cap;
    /* zero the new slots */
    memset((unsigned char *)a_cache + a_cache_used * sizeof(seq_t),
           0, (new_cap - a_cache_used) * sizeof(seq_t));
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n=0, return 0 */

    /* Allocate cache if first call */
    if (!a_cache) {
        a_cache = (seq_t *)malloc(512 * sizeof(seq_t)); /* initial size */
        a_cache_cap = 512;
        memset(a_cache, 0, a_cache_cap * sizeof(seq_t));
        a_cache_used = 0; /* no terms yet */
    }

    if (n <= a_cache_used) return a_cache[(size_t)n];

    /* Prepare tables and DP arrays */
    init_transitions();
    init_dp_arrays();

    /* Expand cache to hold up to n */
    ensure_a_cache(n);

    /* Compute up to required n */
    idx_t target_len = n + 3;   /* length of sequences we need to reach */
    /* Loop from current_length+1 up to target_len */
    for (idx_t L = current_length + 1; L <= target_len; ++L) {
        /* Zero nextdp */
        memset(nextdp, 0, 512 * sizeof(seq_t));

        /* Transition step */
        for (size_t s = 0; s < 512; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            uint8_t cnt = trans_count[s];
            for (uint8_t t = 0; t < cnt; ++t) {
                uint16_t ns = trans_succ[s][t];
                nextdp[ns] += val;
            }
        }

        /* Compute a(L-3) = sum of nextdp */
        seq_t total = 0;
        for (size_t s = 0; s < 512; ++s) {
            total += nextdp[s];
        }
        size_t idx = (size_t)(L - 3);
        a_cache[idx] = total;
        if (idx > a_cache_used) a_cache_used = idx;

        /* Prepare for next iteration */
        seq_t *tmp = dp;
        dp = nextdp;
        nextdp = tmp;
    }

    current_length = target_len;
    return a_cache[(size_t)n];
}
