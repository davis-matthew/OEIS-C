/*
 * A283413  Number of n X 6 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A283413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283413: Number of n×6 binary arrays with no 1 having more than three
   of its horizontal, vertical, and antidiagonal neighbors.
   The function computes the term by dynamic programming over rows. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 64                     /* 6‑bit rows   */
#define STATE (ROWS * ROWS)         /* (prev,row) pairs */

/* Transition table: allowed_next[p][q] has a bit r set iff a row r
   may follow rows p (above) and q (current). */
static uint64_t allowed_next[ROWS][ROWS];
static bool init_allowed = false;

/* Return true iff every 1 in row q has at most 3 of its horizontal,
   vertical and antidiagonal neighbours set to 1, given the upper row
   p and the lower row r. */
static bool row_valid(idx_t p, idx_t q, idx_t r)
{
    for (idx_t j = 0; j < 6; ++j)
    {
        if (q & ((idx_t)1 << j))
        {
            idx_t cnt = 0;
            /* horizontal */
            if (j > 0 && (q & ((idx_t)1 << (j - 1)))) ++cnt;
            if (j < 5 && (q & ((idx_t)1 << (j + 1)))) ++cnt;
            /* vertical */
            if (p & ((idx_t)1 << j)) ++cnt;
            if (r & ((idx_t)1 << j)) ++cnt;
            /* antidiagonal */
            if (j < 5 && (p & ((idx_t)1 << (j + 1)))) ++cnt;
            if (j > 0 && (r & ((idx_t)1 << (j - 1)))) ++cnt;
            if (cnt > 3) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Build transition table once. */
    if (!init_allowed)
    {
        init_allowed = true;
        for (int p = 0; p < ROWS; ++p)
        {
            for (int q = 0; q < ROWS; ++q)
            {
                uint64_t mask = 0;
                for (int r = 0; r < ROWS; ++r)
                    if (row_valid((idx_t)p, (idx_t)q, (idx_t)r))
                        mask |= (uint64_t)1 << r;
                allowed_next[p][q] = mask;
            }
        }
    }

    /* Allocate DP arrays once. */
    static seq_t *dp = NULL;
    static seq_t *dpn = NULL;
    if (!dp)
    {
        dp  = (seq_t*)malloc(STATE * sizeof(seq_t));
        dpn = (seq_t*)malloc(STATE * sizeof(seq_t));
    }

    /* Initialise for the first row: prev=0, curr=any. */
    memset(dp, 0, STATE * sizeof(seq_t));
    for (int q = 0; q < ROWS; ++q)
        dp[q] = (seq_t)1;      /* state (0,q) */

    /* Process subsequent rows. */
    for (idx_t step = 1; step < n; ++step)
    {
        memset(dpn, 0, STATE * sizeof(seq_t));
        for (int p = 0; p < ROWS; ++p)
        {
            for (int q = 0; q < ROWS; ++q)
            {
                seq_t cur = dp[p * ROWS + q];
                if (cur == 0) continue;

                uint64_t mask = allowed_next[p][q];
                while (mask)
                {
                    uint64_t bit = mask & -mask;
                    int r = (int)__builtin_ctzll(bit);
                    dpn[q * ROWS + r] += cur;
                    mask ^= bit;
                }
            }
        }
        /* Prepare for next step. */
        seq_t *tmp = dp; dp = dpn; dpn = tmp;
    }

    /* Final check: last row must be valid with below zero. */
    seq_t result = (seq_t)0;
    for (int p = 0; p < ROWS; ++p)
    {
        for (int q = 0; q < ROWS; ++q)
        {
            if (allowed_next[p][q] & 1ULL)        /* r == 0 */
                result += dp[p * ROWS + q];
        }
    }
    return result;
}
