/*
 * A365646  a(n) is the permanent of the n X n matrix M(n) whose generic M[i,
 *          j] = j*(10^i - 1)/9 with 1 <= i, j <= n.
 * https://oeis.org/A365646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1}^n i^2 * repunit_i * where repunit_i = (10^i -
 *            1)/9 = 1, 11, 111, ... * Derived from perm(M) = (n!)^2 *
 *            Π_{i=1}^n repunit_i * * The function iteratively builds
 *            repunit_i and multiplies * i^2*repunit_i to the running product.
 *            It works for any build * where seq_t supports standard int...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 (empty matrix permanent) */
    if (n == 0) return (seq_t)1;

    seq_t result = 1;      /* running product of terms */
    seq_t repunit = 1;     /* repunit_i, starting with i=1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;          /* cast idx_t to seq_t */
        seq_t i2   = i_seq * i_seq;      /* i^2   */
        seq_t term = i2 * repunit;       /* i^2 * repunit_i */
        result *= term;                  /* accumulate product */
        /* prepare repunit_{i+1} = repunit_i * 10 + 1 */
        repunit = repunit * (seq_t)10 + (seq_t)1;
    }
    return result;
}
