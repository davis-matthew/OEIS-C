/*
 * A357451  Number of compositions (ordered partitions) of n into tribonacci
 *          numbers 1,2,4,7,13,24, ... (A000073).
 * https://oeis.org/A357451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A357451
   Number of compositions of n into tribonacci numbers 1,2,4,7,13,24,...
   Computation: dynamic programming.
   a(0) = 1
   a(n) = Σ_{t tribonacci, t ≤ n} a(n - t)
*/

#include <stdlib.h>

static seq_t *dp          = NULL;   /* memoized DP array */
static size_t dp_cap      = 0;       /* allocated capacity of dp */
static idx_t  dp_upto     = 0;       /* highest index computed in dp */

static idx_t *trib         = NULL;   /* tribonacci numbers ≤ current need */
static size_t trib_cnt     = 0;      /* number of stored tribonacci numbers */
static size_t trib_cap     = 0;      /* allocated capacity of trib array */

/* Ensure that trib[] contains all tribonacci numbers ≤ max_n. */
static void ensure_tribs(idx_t max_n) {
    if (trib == NULL) { /* initial allocation */
        trib_cap = 8;
        trib = (idx_t *)malloc(trib_cap * sizeof(idx_t));
        trib_cnt = 0;
        trib[trib_cnt++] = 1;  /* trib(0) */
        trib[trib_cnt++] = 2;  /* trib(1) */
        trib[trib_cnt++] = 4;  /* trib(2) */
    }
    /* Generate further tribonacci numbers until exceeding max_n */
    while (1) {
        /* Next tribonacci term from the last three */
        if (trib_cnt < 3) break;   /* safety, should never happen */
        idx_t next = trib[trib_cnt-1] + trib[trib_cnt-2] + trib[trib_cnt-3];
        if (next > max_n) break;
        if (trib_cnt == trib_cap) {
            trib_cap = trib_cap * 2;
            trib = (idx_t *)realloc(trib, trib_cap * sizeof(idx_t));
        }
        trib[trib_cnt++] = next;
    }
}

/* Ensure that dp[] is allocated with capacity ≥ n+1. */
static void ensure_dp(idx_t n) {
    if (dp == NULL) {                 /* first allocation */
        dp_cap = n + 1;
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        dp[0] = 1;                    /* a(0) = 1 */
        dp_upto = 0;
    } else if (n + 1 > dp_cap) {      /* need to grow */
        dp_cap = n + 1;
        dp = (seq_t *)realloc(dp, dp_cap * sizeof(seq_t));
    }
}

/* Main function requested by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return 1;            /* a(0) = 1 */

    ensure_dp(n);
    ensure_tribs(n);

    /* Compute values up to n that are not yet known. */
    for (idx_t i = dp_upto + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < trib_cnt; ++j) {
            idx_t t = trib[j];
            if (t > i) break;
            sum += dp[i - t];
        }
        dp[i] = sum;
    }
    dp_upto = n;
    return dp[n];
}
