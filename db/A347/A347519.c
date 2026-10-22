/*
 * A347519  Square array read by antidiagonals upwards taken the terms of
 *          A347270 and replacing the powers of 2 with zeros and the nonpowers
 *          of 2 with ones, n >= 1, k >= 0.
 * https://oeis.org/A347519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A347519: read of the infinite Collatz array A347270 by
 * antidiagonals upwards and replacing powers of 2 by 0 and all other
 * values by 1.
 *
 * Let (i,j) be the coordinates in the array corresponding to the
 * linear index n (1-indexed).  The array elements are
 *   T(i,0) = i
 *   T(i,k+1) = 3*T(i,k)+1 if T(i,k) is odd, otherwise T(i,k)/2.
 * The procedure computes T(i,j) and returns
 *   0  if T(i,j) is a power of two,
 *   1  otherwise.
 *
 * The coordinates (i,j) are determined by reading the array along
 * antidiagonals, with each diagonal read from the bottom element up.
 * The mapping from n to (i,j) is:
 *   s = floor( (sqrt(8n-7)-1)/2 )   // diagonal index, s≥0
 *   p = n - s*(s+1)/2              // position within the diagonal, 1 ≤ p ≤ s+1
 *   i = s + 2 - p                  // row index (1‑based)
 *   j = p - 1                      // column index (0‑based)
 *
 * The implementation below computes s by a simple loop and uses the
 * provided utility `is_power_of_two` to test the final value.
 */
 
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Find diagonal index s such that n lies in the s-th anticeddial. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 < n)
        ++s;

    /* Cumulative count of elements before this diagonal. */
    idx_t cumulative_before = s * (s + 1) / 2;
    idx_t p = n - cumulative_before;            /* position within diagonal, 1 .. s+1 */

    /* Convert to coordinates. Rows are 1‑based, columns 0‑based. */
    idx_t i = s + 2 - p;                         /* row index */
    idx_t j = p - 1;                             /* column index */

    /* Compute the j-th element of the Collatz sequence starting from i. */
    seq_t val = i;
    for (idx_t k = 0; k < j; ++k) {
        if (val & 1)        /* odd */
            val = val * 3 + 1;
        else                /* even */
            val = val / 2;
    }

    return is_power_of_two(val) ? 0 : 1;
}
