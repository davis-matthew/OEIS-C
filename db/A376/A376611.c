/*
 * A376611  Irregular triangle read by rows: T(n,k) is the largest divisor of
 *          binomial(n,k) which is less than n, with 0 <= k <= n.
 * https://oeis.org/A376611
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5251, u16 n <= 5251, u32 n <= 5251, u64 n <= 5251,
 *            u128 n <= 5251, big any n
 * Helpers:   divides_binom, largest_divisor_of_binom_less_than_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A376611: irregular triangle read by rows.
 * a(n) = largest divisor of binomial(row,row_k) that is < row,
 * where (row,k) is the position of the (n-2)‑th element of the
 * concatenated triangle starting with row 2.
 */

#include <stdbool.h>
#include "../utils/divides_binom.h"
#include "../utils/largest_divisor_of_binom_less_than_n.h"

 

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined below offset */

    /* map n to (row, col) in the triangle */
    idx_t idx = n - 2;   /* zero‑based position in concatenated rows */
    idx_t row = 2;
    while (idx >= row + 1) {     /* row has row+1 terms (k=0..row) */
        idx -= row + 1;
        ++row;
    }
    idx_t col = idx;                     /* 0 <= col <= row */

    return largest_divisor_of_binom_less_than_n(row, col);
}
