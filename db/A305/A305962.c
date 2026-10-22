/*
 * A305962  Number A(n,k) of length-n restricted growth strings (RGS) with
 *          growth <= k and fixed first element; square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A305962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 129, u64 n <= 268,
 *            u128 n <= 620, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A305962 values read along antidiagonals of the square array A(n,k).
   The function first determines the pair (n,k) corresponding to the
   antidiagonal index `n`, then counts the restricted growth strings of
   length `n` with growth bound `k` by dynamic programming.
   The recurrence is:
     let C[i][m] = number of strings of length i with current maximum m.
     C[1][1] = 1
     for i > 1:
        C[i][m] += C[i-1][m] * m                  // stay with max m
        for t = m+1 .. min(m+k, new_max):
            C[i][t] += C[i-1][m]                  // new maximum t
   The answer is sum_m C[n][m].
   This implementation stays well below 2 s for indices up to ~10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t idx) {
    /* Handle the trivial case n=0 */
    if (idx == 0) return (seq_t)1;

    /* Step 1: determine (n,k) from the antidiagonal index */
    idx_t s = 0;
    /* Find the smallest s with (s+1)(s+2)/2 > idx */
    while ((s + 1) * (s + 2) / 2 <= idx) ++s;

    /* Start of the current diagonal */
    idx_t prev = s * (s + 1) / 2;          /* number of elements before this diagonal */
    idx_t offset = idx - prev;             /* position inside the diagonal */
    idx_t n_len = offset;                  /* n = offset */
    idx_t k_len = s - offset;              /* k = s - offset */

    /* Step 2: dynamic programming to compute A(n_len, k_len) */
    if (n_len == 0) {
        /* A(0,k) = 1 by definition */
        return (seq_t)1;
    }

    /* Allocate arrays for previous and current DP layers */
    idx_t max_prev = 1;                     /* maximum value seen after first element */
    seq_t *cnt_prev = (seq_t *)calloc(max_prev + 1, sizeof(seq_t));
    seq_t *cnt_curr = NULL;
    cnt_prev[1] = (seq_t)1;                 /* only one string of length 1 with max 1 */

    for (idx_t i = 2; i <= n_len; ++i) {
        idx_t max_curr = max_prev + k_len;  /* maximum possible value after adding element i */
        cnt_curr = (seq_t *)calloc(max_curr + 1, sizeof(seq_t));

        for (idx_t m = 1; m <= max_prev; ++m) {
            seq_t cnt = cnt_prev[m];
            if (cnt == 0) continue;

            /* Stay with the same maximum m */
            cnt_curr[m] += cnt * (seq_t)m;

            /* New maximums from m+1 up to m+k_len */
            idx_t upper = m + k_len;
            if (upper > max_curr) upper = max_curr;
            for (idx_t t = m + 1; t <= upper; ++t) {
                cnt_curr[t] += cnt;
            }
        }

        /* Prepare for next iteration */
        free(cnt_prev);
        cnt_prev = cnt_curr;
        max_prev = max_curr;
    }

    /* Sum all counts to get the final answer */
    seq_t result = 0;
    for (idx_t m = 1; m <= max_prev; ++m) {
        result += cnt_prev[m];
    }

    free(cnt_prev);
    return result;
}
