/*
 * A282646  Number of nX7 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors.
 * https://oeis.org/A282646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A282646: number of n×7 0‑1 arrays with no 1 having more
 * than one king‑move neighbour.  The algorithm builds the matrix
 * row by row, keeping a sliding window of three consecutive rows
 * and using dynamic programming over the possible states of the two
 * most recent rows.  For each step it transitions only to rows that
 * keep the middle row valid.  The DP table has size 128×128 = 16384
 * states.  The routine works for any seq_t/idx_t provided by the
 * build environment. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH   7U
#define PATS    (1U << WIDTH)          /* 128 */
#define STATE   (PATS * PATS)          /* 16384 */

static bool *valid_first = NULL;    /* row a with below b */
static bool *valid_last  = NULL;    /* row b with above a */
static uint64_t *mask0   = NULL;    /* low 64 bits of valid transitions from (a,b) */
static uint64_t *mask1   = NULL;    /* high bits of valid transitions from (a,b) */
static bool *valid_single = NULL;   /* row valid with no neighbours (n==1) */
static bool initialized   = false;

/* Test if a row meets the adjacency condition when its upper neighbour
 * is 'above' and its lower neighbour is 'below'.  Use -1 to mean no
 * neighbour.  */
static inline bool valid_row(uint8_t row, int8_t above, int8_t below)
{
    for (int j = 0; j < WIDTH; ++j) {
        if (!(row & (1U << j))) continue;
        int cnt = 0;
        if (j > 0 && (row & (1U << (j - 1)))) ++cnt;
        if (j < WIDTH - 1 && (row & (1U << (j + 1)))) ++cnt;
        if (above != -1) {
            if (j > 0 && (above & (1U << (j - 1)))) ++cnt;
            if (above & (1U << j)) ++cnt;
            if (j < WIDTH - 1 && (above & (1U << (j + 1)))) ++cnt;
        }
        if (below != -1) {
            if (j > 0 && (below & (1U << (j - 1)))) ++cnt;
            if (below & (1U << j)) ++cnt;
            if (j < WIDTH - 1 && (below & (1U << (j + 1)))) ++cnt;
        }
        if (cnt > 1) return false;
    }
    return true;
}

/* Pre‑compute tables used by the DP.  This routine is called lazily. */
static void init_precompute(void)
{
    if (initialized) return;
    size_t i, j, k;
    valid_first   = (bool *)malloc(STATE * sizeof(bool));
    valid_last    = (bool *)malloc(STATE * sizeof(bool));
    mask0         = (uint64_t *)malloc(STATE * sizeof(uint64_t));
    mask1         = (uint64_t *)malloc(STATE * sizeof(uint64_t));
    valid_single  = (bool *)malloc(PATS * sizeof(bool));

    for (i = 0; i < PATS; ++i)
        valid_single[i] = valid_row((uint8_t)i, -1, -1);

    for (i = 0; i < PATS; ++i) {
        for (j = 0; j < PATS; ++j) {
            size_t id = i * PATS + j;
            valid_first[id] = valid_row((uint8_t)i, -1, (int8_t)j);
            valid_last[id]  = valid_row((uint8_t)j, (int8_t)i, -1);
            uint64_t m0 = 0, m1 = 0;
            for (k = 0; k < PATS; ++k) {
                if (valid_row((uint8_t)j, (int8_t)i, (int8_t)k)) {
                    if (k < 64) m0 |= 1ULL << k;
                    else        m1 |= 1ULL << (k - 64);
                }
            }
            mask0[id] = m0;
            mask1[id] = m1;
        }
    }
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_precompute();

    if (n == 0) return (seq_t)0;   /* undefined below offset 1 */

    /* n == 1: count single rows with no neighbours */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t i = 0; i < PATS; ++i)
            if (valid_single[i]) ++ans;
        return ans;
    }

    /* n == 2: count first‑row/second‑row pairs that satisfy constraints */
    if (n == 2) {
        seq_t ans = 0;
        for (size_t id = 0; id < STATE; ++id)
            if (valid_first[id] && valid_last[id]) ++ans;
        return ans;
    }

    /* n >= 3: dynamic programming over three rows */
    seq_t *dp = (seq_t *)calloc(STATE, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATE, sizeof(seq_t));
    if (!dp || !dp_next) return (seq_t)0; /* allocation failure unlikely */

    /* initialise with all valid first pairs (row0, row1) */
    for (size_t id = 0; id < STATE; ++id)
        if (valid_first[id]) dp[id] = (seq_t)1;

    /* add rows 3 .. n */
    for (idx_t step = 2; step < n; ++step) {
        for (size_t t = 0; t < STATE; ++t) dp_next[t] = (seq_t)0;
        for (size_t id = 0; id < STATE; ++id) {
            seq_t cur = dp[id];
            if (cur == (seq_t)0) continue;
            uint8_t b = (uint8_t)(id & (PATS - 1));
            uint64_t m0 = mask0[id];
            uint64_t m1 = mask1[id];
            /* process low part of transition mask */
            while (m0) {
                int c = __builtin_ctzll(m0);
                size_t nid = ((size_t)b * PATS) + c;
                dp_next[nid] += cur;
                m0 &= m0 - 1;
            }
            /* high part */
            while (m1) {
                int c = 64 + __builtin_ctzll(m1);
                size_t nid = ((size_t)b * PATS) + c;
                dp_next[nid] += cur;
                m1 &= m1 - 1;
            }
        }
        /* swap dp arrays */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* final row must be valid with no lower neighbour */
    seq_t result = 0;
    for (size_t id = 0; id < STATE; ++id)
        if (valid_last[id] && dp[id] != (seq_t)0)
            result += dp[id];

    free(dp);
    free(dp_next);
    return result;
}
