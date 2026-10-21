/*
 * A274607  Triangle read by rows: T(n, k) is the largest number that can be
 *          formed by multiplying k primes prime(i1),...,prime(ik) such that
 *          i1+...+ik = n.
 * https://oeis.org/A274607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime, pow2, seq_cmp, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A274607 by dynamic programming over prime products. */
/* For each n, a(n) is the maximum product of k primes whose indices sum to r,
   where r and k satisfy r*(r-1)/2 < n <= r*(r+1)/2. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column c of the antidiagonal that contains n. */
    idx_t r = 1;
    while ( ((r * (r + 1)) / 2) < n ) r++;

    idx_t prev_sum = ((r - 1) * r) / 2;
    idx_t c = n - prev_sum;                     /* 1‑based column */

    /* Static dynamic programming table: dp[row][col] holds the best product. */
    static seq_t **dp = NULL;
    static idx_t dp_max_row = 0;

    /* Extend the table until it contains the required row r. */
    while (dp_max_row < r) {
        idx_t new_row = ++dp_max_row;

        dp = (seq_t **)realloc(dp, sizeof(seq_t *) * (dp_max_row + 1));
        dp[new_row] = (seq_t *)calloc((new_row + 1), sizeof(seq_t));  /* indices 1..new_row */

        /* Base cases. */
        dp[new_row][1]  = (seq_t)get_prime(new_row);  /* one prime with index sum new_row */
        dp[new_row][new_row] = pow2(new_row);        /* all factors 2 (k = new_row) */

        /* Fill the interior entries. */
        for (idx_t k = 2; k < new_row; ++k) {
            seq_t best = 0;
            int first = 1;
            /* Choose the first prime index i (1 <= i <= new_row - (k-1)) */
            for (idx_t i = 1; i <= new_row - (k - 1); ++i) {
                seq_t cand = seq_mul((seq_t)get_prime(i), dp[new_row - i][k - 1]);

                if (first) {
                    best = cand;
                    first = 0;
                } else {
                    if (seq_cmp((const void *)&cand, (const void *)&best) > 0) {
                        /* cand is larger; replace best. */
                        best = cand;
                    }
                }
            }
            dp[new_row][k] = best;
        }
    }

    return dp[r][c];
}
