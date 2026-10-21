/*
 * A250156  Number of length 2+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 14, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A250156: Number of length 2+6 0..n arrays with every
 * seven consecutive terms having the maximum of some two terms equal
 * to the minimum of the remaining five terms.
 * 
 * The sequence is a polynomial of degree 7:
 *   a(n) = (22*n^7 + 182*n^6 + 469*n^5 + 679*n^4 + 
 *          721*n^3 + 476*n^2 + 139*n + 14) / 14
 * This formula holds for all n ≥ 1 (offset 1).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure n >= 1; for n==0 the value is undefined, but we return 0 */
    if (n == 0) return (seq_t)0;

    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    seq_t n6 = n5 * n1;
    seq_t n7 = n6 * n1;

    seq_t num = 22 * n7
              + 182 * n6
              + 469 * n5
              + 679 * n4
              + 721 * n3
              + 476 * n2
              + 139 * n1
              + 14;

    return num / 14;
}
