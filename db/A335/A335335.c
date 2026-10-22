/*
 * A335335  Irregular triangle T(n,k) of Arnold numbers with n>=1 and 1<=
 *          abs(k) <= n.
 * https://oeis.org/A335335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   idx_of_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implementation of OEIS A335335 – irregular triangle T(n,k) of Arnold numbers.
 *
 * Recurrence (from OEIS):
 *   T(1,±1) = 1,  T(n,-n) = 0  (n ≥ 2)
 *   If n ≥ k > 1:       T(n,k) = T(n,k-1) + T(n-1,-k+1)
 *   If n > k = 1:       T(n,k) = T(n,-1)
 *   If -1 ≥ k > -n:     T(n,k) = T(n,k-1) + T(n-1,-k)
 *
 * The sequence a(n) enumerates T(n,k) along antidiagonals:
 *   for each row n ≥ 1, k runs from -n, -(n-1), … , -1, 1, … , n
 *   a(1) = T(1,1), a(2) = T(1,-1), a(3) = T(2,-2), …
 *
 * This implementation builds successive rows up to the required row
 * using the recurrence and returns the element that corresponds to the
 * given index n in the sequence (offset 1).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/idx_of_k.h"

/* OEIS sequence A335335 */
seq_t a(idx_t n) {
    /* Find the row m that contains the n-th term.  Total elements up to
       row R is R*(R+1).  n is 1‑based. */
    idx_t m = 1;
    for (; m * (m + 1) < n; ++m)
        ;
    /* Position within row m (1 … 2m) */
    idx_t prev_total = (m - 1) * m;          /* elements before row m */
    idx_t pos = n - prev_total;              /* 1‑based position in row m */

    /* Compute k from position */
    int k;
    if (pos <= m) {
        /* negative side: -m … -1 */
        k = - (int)(m - pos + 1);
    } else {
        /* positive side: 1 … m */
        k = (int)(pos - m);
    }

    /* Allocate the first row (row 1) */
    seq_t *prev = (seq_t *)malloc(2 * sizeof(seq_t));
    prev[0] = 1;   /* k = –1 */
    prev[1] = 1;   /* k = +1 */

    /* Build rows 2 … m */
    for (int row = 2; row <= (int)m; ++row) {
        size_t curr_len = (size_t)2 * row;
        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));

        /* Negative k: k = –row … –1 */
        /* T(row, –row) = 0 */
        size_t idx_cur = idx_of_k(-row, row);
        curr[idx_cur] = 0;

        for (int kk = -row + 1; kk <= -1; ++kk) {
            size_t idx_curr = idx_of_k(kk, row);
            size_t idx_curr_prev = idx_of_k(kk - 1, row);
            size_t idx_prev = idx_of_k(-kk, row - 1);
            curr[idx_curr] = curr[idx_curr_prev] + prev[idx_prev];
        }

        /* Positive k: k = 1 … row */
        /* T(row, 1) = T(row, –1) */
        size_t idx_pos1 = idx_of_k(1, row);
        size_t idx_neg1 = idx_of_k(-1, row);
        curr[idx_pos1] = curr[idx_neg1];

        for (int kk = 2; kk <= row; ++kk) {
            size_t idx_curr_val = idx_of_k(kk, row);
            size_t idx_curr_prev2 = idx_of_k(kk - 1, row);
            size_t idx_prev_val = idx_of_k(-(kk - 1), row - 1);
            curr[idx_curr_val] = curr[idx_curr_prev2] + prev[idx_prev_val];
        }

        /* Move to next iteration */
        free(prev);
        prev = curr;
    }

    /* Retrieve the requested element */
    size_t idx_ans = idx_of_k(k, (int)m);
    seq_t result = prev[idx_ans];
    free(prev);
    return result;
}
