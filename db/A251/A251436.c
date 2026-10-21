/*
 * A251436  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A251436
 * a(n) = number of (n+1)×3 arrays with entries 0..3 for which no 2×2 sub‑block has
 * the minimum of its diagonal elements greater than the absolute difference of
 * its antidiagonal elements.
 * Computed by dynamic programming over the 64 possible column states of a
 * width‑3 slice (each column cell can be 0..3).
 * The DP state transition matrix (64×64) is precomputed; each step adds one
 * new row to the array.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define STATES 64

/* Transition matrix: T[i][j] = 1 if state i can be followed by state j */
static seq_t T[STATES][STATES];
static bool T_inited = false;

/* Build the transition matrix once. */
static void build_T(void) {
    for (int i = 0; i < STATES; ++i) {
        int a0 = i / 16;          /* left column value of state i */
        int a1 = (i / 4) % 4;     /* middle column value of state i */
        int a2 = i % 4;           /* right column value of state i */
        for (int j = 0; j < STATES; ++j) {
            int b0 = j / 16;
            int b1 = (j / 4) % 4;
            int b2 = j % 4;

            /* Left 2×2 block (columns 1&2) */
            int diag_min1 = (a0 < b1) ? a0 : b1;
            int anti_diff1 = (a1 >= b0) ? a1 - b0 : b0 - a1;
            bool ok1 = diag_min1 <= anti_diff1;

            /* Right 2×2 block (columns 2&3) */
            int diag_min2 = (a1 < b2) ? a1 : b2;
            int anti_diff2 = (a2 >= b1) ? a2 - b1 : b1 - a2;
            bool ok2 = diag_min2 <= anti_diff2;

            T[i][j] = (ok1 && ok2) ? 1 : 0;
        }
    }
    T_inited = true;
}

/* DP state arrays */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];
static bool dp_inited = false;
static idx_t current_n = 0;          /* number of transitions processed so far */

/* Cache of already computed a(n) values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure cache can hold indices up to n (0‑based). */
static void ensure_cache_capacity(idx_t n) {
    if (cache == NULL || n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* unrecoverable */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* index 0 is outside the offset */

    /* Build transition matrix once. */
    if (!T_inited) build_T();

    /* Initialise DP if first call. */
    if (!dp_inited) {
        for (int i = 0; i < STATES; ++i) dp_cur[i] = 1;   /* first row can be any state */
        dp_inited = true;
        current_n = 0;              /* 0 transitions processed yet (1 row) */
    }

    /* Grow cache if necessary. */
    ensure_cache_capacity(n - 1);

    /* Extend DP to reach desired n.  Each step adds one transition (one more row). */
    while (current_n < n) {
        /* Compute dp_next = dp_cur * T  (matrix–vector multiplication). */
        for (int j = 0; j < STATES; ++j) dp_next[j] = 0;
        for (int i = 0; i < STATES; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            for (int j = 0; j < STATES; ++j) {
                if (T[i][j]) dp_next[j] += val;
            }
        }

        /* Sum over all ending states to obtain a(current_n + 1). */
        seq_t sum = 0;
        for (int j = 0; j < STATES; ++j) sum += dp_next[j];
        cache[current_n] = sum;       /* a(current_n + 1) stored at index current_n */

        /* Prepare for next iteration. */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
        ++current_n;
    }

    return cache[n - 1];
}
