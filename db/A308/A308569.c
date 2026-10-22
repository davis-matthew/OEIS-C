/*
 * A308569  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{d|n} d^(k*n).
 * https://oeis.org/A308569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 31, u64 n <= 59, u128 n <= 97,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements sequence A308569:  a(n) = sum_{d|row} d^(row*k)
   where (row,k) is obtained from the n-th term in antidiagonal order
   of the square array A(row,k) = sum_{d|row} d^(k*row).
   The mapping from n to (row,k) is: 
       let s be the smallest integer with (s+1)(s+2)/2 >= n,
       let r = n - s(s+1)/2 - 1   (0‑based offset in the s‑th diagonal),
       row = r + 1,
       k   = s - r.
   Then a(n) = Σ_{d|row} d^(row*k).
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find diagonal sum s such that s(s+1)/2 < n ≤ (s+1)(s+2)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 < n)
        ++s;

    idx_t prevCount = s * (s + 1) / 2;
    idx_t r        = n - prevCount - 1;   /* 0‑based offset in diagonal */
    idx_t row      = r + 1;               /* 1‑based row index */
    idx_t k        = s - r;               /* column index */

    seq_t result = 0;

    /* Enumerate divisors of row (which is ≤ 10^5 for n ≤ 10^9) */
    for (idx_t d = 1; d * d <= row; ++d)
    {
        if (row % d == 0)
        {
            /* Compute d^(row*k) efficiently as (d^row)^k */
            seq_t base = big_pow((seq_t)d, (seq_t)row);
            seq_t term = big_pow(base, (seq_t)k);
            result += term;

            idx_t e = row / d;
            if (e != d)
            {
                base = big_pow((seq_t)e, (seq_t)row);
                term = big_pow(base, (seq_t)k);
                result += term;
            }
        }
    }

    return result;
}
