/*
 * A251848  Number of (n+2)X(3+2) 0..3 arrays with every row, column, diagonal
 *          or antidiagonal in each 3X3 subblock summing to a prime.
 * https://oeis.org/A251848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A251848: number of (n+2)×5 arrays with entries 0..3 such that in every
 * 3×3 subblock each row, each column, the main diagonal, and the anti‑diagonal
 * sum to a prime (2,3,5,7).  The array has (n+2) rows and 5 columns.
 * The function computes the count for arbitrary n ≥ 1 by dynamic programming.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_sum.h"

/* main function required by the harness */
seq_t a(idx_t n)
{
    /* ---------- static data, built once --------------------------------- */
    static bool built = false;
    static int R;                     /* number of admissible rows */
    static uint8_t *row_digits;       /* R * 5, digits of each row   */
    static int *adj_start;            /* start index in adjacency buffer for state (i,j) */
    static int *adj_len;              /* number of successors for state (i,j) */
    static int *adj_buf;              /* concatenated successor lists   */
    static seq_t *dp1, *dp2;          /* DP arrays, size = R*R          */
    static size_t M;                  /* number of states = R*R         */

    if (!built)
    {
        /* ---- enumerate all admissible 5‑digit rows ---------------------- */
        int tmp_idx[1024];           /* 4^5 = 1024 possible rows       */
        int cnt = 0;
        for (int val = 0; val < 1024; ++val)
        {
            uint8_t d[5];
            int x = val;
            for (int i = 0; i < 5; ++i)
            {
                d[i] = x & 3;
                x >>= 2;
            }
            bool ok = true;
            for (int c = 0; c < 3; ++c)
            {
                int s = d[c] + d[c + 1] + d[c + 2];
                if (!is_prime_sum(s))
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
                tmp_idx[cnt++] = val;
        }
        R = cnt;

        /* ---- store digits of admissible rows --------------------------- */
        row_digits = (uint8_t *)malloc(R * 5 * sizeof(uint8_t));
        for (int i = 0; i < R; ++i)
        {
            int val = tmp_idx[i];
            int x = val;
            for (int c = 0; c < 5; ++c)
            {
                row_digits[i * 5 + c] = x & 3;
                x >>= 2;
            }
        }

        /* ---- prepare adjacency structures -------------------------------- */
        M = (size_t)R * (size_t)R;        /* number of states (i,j)             */
        adj_start = (int *)malloc(M * sizeof(int));
        adj_len   = (int *)malloc(M * sizeof(int));
        adj_buf   = (int *)malloc(M * R * sizeof(int));

        int buf_pos = 0;
        for (int i = 0; i < R; ++i)
        {
            for (int j = 0; j < R; ++j)
            {
                int start = buf_pos;
                int len   = 0;
                for (int k = 0; k < R; ++k)
                {
                    bool ok = true;
                    for (int left = 0; left < 3 && ok; ++left)
                    {
                        /* column sums */
                        int col0 = row_digits[i * 5 + left]
                                   + row_digits[j * 5 + left]
                                   + row_digits[k * 5 + left];
                        if (!is_prime_sum(col0)) { ok = false; break; }
                        int col1 = row_digits[i * 5 + left + 1]
                                   + row_digits[j * 5 + left + 1]
                                   + row_digits[k * 5 + left + 1];
                        if (!is_prime_sum(col1)) { ok = false; break; }
                        int col2 = row_digits[i * 5 + left + 2]
                                   + row_digits[j * 5 + left + 2]
                                   + row_digits[k * 5 + left + 2];
                        if (!is_prime_sum(col2)) { ok = false; break; }

                        /* diagonals */
                        int diag1 = row_digits[i * 5 + left]
                                    + row_digits[j * 5 + left + 1]
                                    + row_digits[k * 5 + left + 2];
                        if (!is_prime_sum(diag1)) { ok = false; break; }
                        int diag2 = row_digits[i * 5 + left + 2]
                                    + row_digits[j * 5 + left + 1]
                                    + row_digits[k * 5 + left];
                        if (!is_prime_sum(diag2)) { ok = false; break; }
                    }
                    if (ok)
                    {
                        adj_buf[buf_pos++] = k;
                        ++len;
                    }
                }
                adj_start[i * R + j] = start;
                adj_len[i * R + j]   = len;
            }
        }

        /* ---- DP arrays --------------------------------------------------- */
        dp1 = (seq_t *)calloc(M, sizeof(seq_t));
        dp2 = (seq_t *)calloc(M, sizeof(seq_t));

        built = true;
    }

    /* ---- initialise DP: every pair of rows is a valid start ------- */
    for (size_t s = 0; s < M; ++s)
        dp1[s] = (seq_t)1;

    /* ---- perform DP for n steps ------------------------------------- */
    idx_t steps = n;                    /* we need to add n further rows  */
    while (steps--)
    {
        /* zero next DP */
        for (size_t s = 0; s < M; ++s)
            dp2[s] = (seq_t)0;

        for (size_t state = 0; state < M; ++state)
        {
            seq_t val = dp1[state];
            if (val == (seq_t)0) continue;
            int start = adj_start[state];
            int len   = adj_len[state];
            int j = (int)(state % R);          /* second row of the pair */
            for (int t = 0; t < len; ++t)
            {
                int k = adj_buf[start + t];
                size_t next = (size_t)j * R + (size_t)k;
                dp2[next] += val;
            }
        }
        /* swap arrays */
        seq_t *tmp = dp1;
        dp1 = dp2;
        dp2 = tmp;
    }

    /* ---- sum all final states ---------------------------------------- */
    seq_t result = (seq_t)0;
    for (size_t s = 0; s < M; ++s)
        result += dp1[s];

    return result;
}
