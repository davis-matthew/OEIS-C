/*
 * A375045  Irregular triangular array T; row n shows the coefficients of the
 *          (n-1)-st polynomial in the obverse convolution s(x)**t(x), where
 *          s(x) = 2^n and t(x) = 2x+1. See Comments.
 * https://oeis.org/A375045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A375045: Irregular triangular array T.
 *  Row n contains the coefficients of the (n‑1)-st polynomial in the
 *  obverse convolution s(x)**t(x) where s(x)=2^n and t(x)=2x+1.
 *
 *  Recurrence for the coefficients a_{n,k} (0 ≤ k ≤ n):
 *      a_{1,0} = a_{1,1} = 2
 *      For n ≥ 1:
 *          a_{n+1,0}         = (1 + 2^n) * a_{n,0}
 *          a_{n+1,k}         = (1 + 2^n) * a_{n,k} + 2 * a_{n,k-1}
 *                            for 1 ≤ k ≤ n
 *          a_{n+1,n+1}       = 2 * a_{n,n}
 *
 *  The sequence is read row‑wise: a(1)=a_{1,0}, a(2)=a_{1,1},
 *  a(3)=a_{2,0}, a(4)=a_{2,1}, etc.
 *
 *  The implementation below computes the n‑th term on demand.
 *  For n ≥ 1 it finds the corresponding row and coefficient index,
 *  then builds rows up to that one by using the above recurrence.
 *
 *  All arithmetic uses the seq_t type;  its operators +, -, *, /,
 *  and the constants satisfy the required properties.
 *
 *  Complexity: O(r^2) for row r where the sought term lies, 
 *  with r ≈ √(2n).  For n up to ~10⁴ this is easily below the
 *  2‑second limit.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* defensive, though offset is 1 */

    /* Find the row r that contains the n-th term.
     * Row lengths are 2,3,4, ... so the cumulative count is
     * S(r) = r(r+3)/2.  Find smallest r with S(r) ≥ n. */
    idx_t r = 1, sum = 2;
    while (sum < n) {
        ++r;
        sum += r + 1;
    }
    /* Terms before this row */
    idx_t prev_sum = sum - (r + 1);
    /* Index inside the row (0‑based) */
    idx_t k = n - prev_sum - 1;

    /* Allocate buffers for two consecutive rows.  The maximum row
     * length needed is r+1. */
    size_t max_len = (size_t)r + 1;
    seq_t *prev = (seq_t *)malloc(max_len * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(max_len * sizeof(seq_t));

    /* Initial row (row 1) has length 2. */
    prev[0] = 2;          /* a_{1,0} */
    prev[1] = 2;          /* a_{1,1} */
    size_t prev_len = 2;  /* current length of prev */

    /* Power of two used as factor 2^i in the recurrence.
     * Initially set to 1; before each iteration it becomes 2^i. */
    seq_t pow_two = 1;

    /* Build rows up to r. */
    for (idx_t i = 1; i < r; ++i) {
        /* pow_two becomes 2^i */
        pow_two = pow_two * (seq_t)2;

        size_t cur_len = prev_len + 1;

        /* Compute the new row from prev */
        cur[0] = prev[0] + pow_two * prev[0];
        for (size_t j = 1; j < prev_len; ++j) {
            cur[j] = prev[j] + pow_two * prev[j] + (seq_t)2 * prev[j - 1];
        }
        cur[cur_len - 1] = (seq_t)2 * prev[prev_len - 1];

        /* Swap buffers for next iteration */
        seq_t *tmp   = prev;
        prev = cur;
        cur  = tmp;
        prev_len = cur_len;
    }

    /* Retrieve the desired coefficient */
    seq_t result = prev[k];

    /* Free allocated memory */
    free(prev);
    free(cur);

    return result;
}
