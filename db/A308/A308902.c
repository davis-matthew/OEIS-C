/*
 * A308902  Number of partitions of n into 6 squarefree parts.
 * https://oeis.org/A308902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Number of partitions of n into 6 square‑free parts.
    The function computes the coefficient of x^n in (∑_{d square‑free}x^d)^6
    by the standard “coin change with fixed number of coins” DP. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Static scratch space shared between calls. */
static idx_t          dp_size      = 0;          /* current DP size   */
static seq_t *        dp[7]        = {NULL};    /* dp[parts][sum]    */
static uint8_t *      is_sq        = NULL;       /* 1 – square‑free   */
static idx_t          sq_size      = 0;          /* size of is_sq     */

/* Build square‑free flag array up to up (inclusive). */
static void
build_squarefree(idx_t up)
{
    if (up <= sq_size)
        return;

    uint8_t *arr = (uint8_t *)realloc(is_sq, (up + 1) * sizeof(uint8_t));
    if (!arr) exit(1);
    is_sq = arr;
    /* initialise with 1 (assume square‑free) */
    for (idx_t i = sq_size + 1; i <= up; ++i)
        is_sq[i] = 1;

    for (idx_t p = 2; p * p <= up; ++p) {
        bool prime = true;
        for (idx_t d = 2; d * d <= p; ++d)
            if (p % d == 0) { prime = false; break; }
        if (!prime)
            continue;
        idx_t sq = p * p;
        for (idx_t m = sq; m <= up; m += sq)
            is_sq[m] = 0;
    }
    sq_size = up;
}

/* Re‑initialise DP tables for size n (first call or new larger n). */
static void
reinit_dp(idx_t n)
{
    for (int parts = 0; parts <= 6; ++parts) {
        seq_t *arr = (seq_t *)realloc(dp[parts], (n + 1) * sizeof(seq_t));
        if (!arr) exit(1);
        dp[parts] = arr;
    }

    /* Zero everything, then set dp[0][0] = 1. */
    for (int parts = 0; parts <= 6; ++parts)
        memset(dp[parts], 0, (n + 1) * sizeof(seq_t));
    dp[0][0] = 1;
    dp_size = n;
}

/* Main function as required by the exercise. */
seq_t a(idx_t n)
{
    /* Ensure we have DP tables and square‑free flags up to n. */
    if (dp[0] == NULL || n > dp_size) {
        build_squarefree(n);
        reinit_dp(n);
    }

    /* DP: for each square‑free d, update counts for all part numbers. */
    for (idx_t d = 1; d <= n; ++d) {
        if (!is_sq[d])
            continue;
        for (int parts = 1; parts <= 6; ++parts) {
            seq_t *prev = dp[parts - 1];
            seq_t *curr = dp[parts];
            for (idx_t s = d; s <= n; ++s)
                curr[s] += prev[s - d];
        }
    }
    return dp[6][n];
}
