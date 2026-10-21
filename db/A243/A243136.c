/*
 * A243136  a(n) = 512*n^10 - 1280*n^8 + 1120*n^6 - 400*n^4 + 50*n^2 - 1.
 * https://oeis.org/A243136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 512*n^10 - 1280*n^8 + 1120*n^6 - 400*n^4 + 50*n^2 - 1
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 4, i64 n <= 42, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t once */
    seq_t n_seq = (seq_t)n;

    /* compute successive powers of n */
    seq_t n2  = n_seq * n_seq;          /* n^2   */
    seq_t n4  = n2 * n2;                /* n^4   */
    seq_t n6  = n4 * n2;                /* n^6   */
    seq_t n8  = n4 * n4;                /* n^8   */
    seq_t n10 = n8 * n2;                /* n^10  */

    /* evaluate the polynomial */
    seq_t val = 512 * n10
              - 1280 * n8
              + 1120 * n6
              - 400  * n4
              + 50   * n2
              - 1;
    return val;
}
