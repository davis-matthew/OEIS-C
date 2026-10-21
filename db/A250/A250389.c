/*
 * A250389  Number of length 3+3 0..n arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (30*n^6 + 32*n^5 + 65*n^4 + 50*n^3 - 5*n^2 + 8*n)/30 for
 *            n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>        /* for compatibility with the build environment */

seq_t a(idx_t n) {
    /* cast the index to seq_t */
    seq_t N = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = N * N;             /* n^2 */
    seq_t n3 = n2 * N;            /* n^3 */
    seq_t n4 = n3 * N;            /* n^4 */
    seq_t n5 = n4 * N;            /* n^5 */
    seq_t n6 = n5 * N;            /* n^6 */

    /* compute the terms of the numerator */
    seq_t t1 = (seq_t)30 * n6;    /* 30*n^6   */
    seq_t t2 = (seq_t)32 * n5;    /* 32*n^5   */
    seq_t t3 = (seq_t)65 * n4;    /* 65*n^4   */
    seq_t t4 = (seq_t)50 * n3;    /* 50*n^3   */
    seq_t t5 = (seq_t)5  * n2;    /* 5*n^2    */
    seq_t t6 = (seq_t)8  * N;     /* 8*n      */

    /* sum the positive terms first to avoid underflow when subtracting */
    seq_t pos_sum = t1 + t2 + t3 + t4 + t6;   /* 30*n^6 + 32*n^5 + 65*n^4 + 50*n^3 + 8*n */
    seq_t numer   = pos_sum - t5;              /* subtract 5*n^2 */

    /* final division by 30 */
    return numer / (seq_t)30;
}
