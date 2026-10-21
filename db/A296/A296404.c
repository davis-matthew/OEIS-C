/*
 * A296404  Number of n X 7 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 2 or 3 neighboring 1's.
 * https://oeis.org/A296404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows of a 7‑column binary array.
   The state keeps the two most recent rows; a transition to the next row
   is allowed iff every 1 in the middle row has exactly 2 or 3 neighbors
   among the two adjacent rows, its left/right neighbors in the same row,
   and the two antidiagonal neighbors.  After processing n rows we
   count states whose next row is all zeros.  This yields the terms of
   OEIS A296404. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define W 7                            /* number of columns */
#define MASK (1U << W)                 /* 2^W = 128 */
#define STATES (MASK * MASK)           /* 128*128 = 16384 */

/* compatibility list for each pair (a,b) */
static uint8_t comp_list[STATES][MASK];
static uint8_t comp_len[STATES];

/* test whether all 1's in row b have 2 or 3 neighbors with rows a (above)
   and c (below) using horizontal, vertical and antidiagonal adjacency */
static inline bool row_ok(uint8_t a, uint8_t b, uint8_t c)
{
    for (int j = 0; j < W; ++j) {
        if ((b >> j) & 1U) {
            int deg = 0;
            /* same row */
            if (j > 0)          deg += (b >> (j - 1)) & 1U;
            if (j < W - 1)      deg += (b >> (j + 1)) & 1U;
            /* above row */
            deg += (a >> j) & 1U;
            if (j < W - 1)      deg += (a >> (j + 1)) & 1U;
            /* below row */
            deg += (c >> j) & 1U;
            if (j > 0)          deg += (c >> (j - 1)) & 1U;
            if (deg != 2 && deg != 3) return false;
        }
    }
    return true;
}

/* precompute compatibility lists */
static void init_comps(void)
{
    for (uint16_t a = 0; a < MASK; ++a) {
        for (uint16_t b = 0; b < MASK; ++b) {
            uint16_t idx = (a << W) | b;
            uint8_t   len = 0;
            for (uint16_t c = 0; c < MASK; ++c) {
                if (row_ok((uint8_t)a, (uint8_t)b, (uint8_t)c))
                    comp_list[idx][len++] = (uint8_t)c;
            }
            comp_len[idx] = len;
        }
    }
}

/* DP arrays: dp_cur[(a<<W)|b] = number of ways to reach rows (a,b) */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
static size_t dp_size = 0;

/* initialise DP storage (handles C and C++ builds) */
static void init_dp(void)
{
    dp_size = STATES;
#ifdef __cplusplus
    dp_cur  = new seq_t[dp_size]();
    dp_next = new seq_t[dp_size]();
#else
    dp_cur  = (seq_t*)calloc(dp_size, sizeof(seq_t));
    dp_next = (seq_t*)calloc(dp_size, sizeof(seq_t));
#endif
}

/* memoised results */
static seq_t *results   = NULL;   /* 1‑based indexes */
static size_t res_cap   = 0;
static idx_t  computed_n = 0;     /* greatest n computed so far */

/* extend result storage to new_cap */
static void grow_results(size_t new_cap)
{
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap]();
    for (size_t i = 0; i < res_cap; ++i) new_arr[i] = results[i];
    delete[] results;
#else
    new_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
    memcpy(new_arr, results, res_cap * sizeof(seq_t));
    free(results);
#endif
    results = new_arr;
    res_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Public function                                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, guard against misuse */

    static bool initialized = false;
    if (!initialized) {
        /* first call: build tables and compute a(1) */
        init_comps();
        init_dp();

        /* base: initial row above is all zeros */
        for (uint16_t b = 0; b < MASK; ++b)
            dp_cur[(0 << W) | b] = 1;

        /* compute a(1) */
        seq_t ans = 0;
        for (uint16_t a = 0; a < MASK; ++a) {
            for (uint16_t b = 0; b < MASK; ++b) {
                seq_t val = dp_cur[(a << W) | b];
                if (val && row_ok((uint8_t)a, (uint8_t)b, 0U))
                    ans += val;
            }
        }

        /* allocate result storage (one‑based) */
        res_cap = 5;  /* start small, will grow as needed */
#ifdef __cplusplus
        results = new seq_t[res_cap]();
#else
        results = (seq_t*)calloc(res_cap, sizeof(seq_t));
#endif
        results[1] = ans;
        computed_n = 1;
        initialized = true;
    }

    /* extend computation up to required n */
    while (computed_n < n) {
        /* clear next layer */
        for (size_t i = 0; i < dp_size; ++i)
            dp_next[i] = 0;

        /* transition from each (a,b) to compatible c */
        for (uint16_t a = 0; a < MASK; ++a) {
            for (uint16_t b = 0; b < MASK; ++b) {
                seq_t val = dp_cur[(a << W) | b];
                if (!val) continue;
                uint16_t idx = (a << W) | b;
                uint8_t   len = comp_len[idx];
                const uint8_t *lst = comp_list[idx];
                for (uint8_t k = 0; k < len; ++k) {
                    uint16_t c = lst[k];
                    dp_next[(b << W) | c] += val;
                }
            }
        }

        /* swap current and next layers */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;

        /* count valid arrays of this size */
        seq_t ans = 0;
        for (uint16_t a = 0; a < MASK; ++a) {
            for (uint16_t b = 0; b < MASK; ++b) {
                seq_t val = dp_cur[(a << W) | b];
                if (val && row_ok((uint8_t)a, (uint8_t)b, 0U))
                    ans += val;
            }
        }

        ++computed_n;

        /* grow result array if necessary */
        if (computed_n >= res_cap) {
            size_t new_cap = res_cap * 2;
            if (new_cap < computed_n + 1) new_cap = computed_n + 1;
            grow_results(new_cap);
        }
        results[ (size_t)computed_n ] = ans;
    }

    return results[(size_t)n];
}
