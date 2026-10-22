/*
 * A346434  Triangle read by rows of numbers with n 1's and n 0's in their
 *          representation in base of Fibonacci numbers (A210619), written as
 *          those 1's and 0's.
 * https://oeis.org/A346434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where r is the smallest integer with r*(r+1)/2 >=
 *            n, c = n - (r-1)*r/2, and T(r,c) = (10*100^r - 9*100^(r-c) -
 *            1)/99
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 55, u128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */
    /* Find row r such that cumulative rows ≥ n. */
    idx_t r   = 1;
    idx_t cum = 1;                  /* cumulative count up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t c = n - (cum - r);        /* column index in row r */

    seq_t res = 0;
    /* Append c pairs of "10". */
    for (idx_t i = 0; i < c; ++i) {
        res = res * 100 + 10;
    }
    /* Append remaining (r-c) pairs of "01". */
    for (idx_t i = c; i < r; ++i) {
        res = res * 100 + 1;
    }
    return res;
}
