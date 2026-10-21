/*
 * A250338  Number of length 2+5 0..n arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 9*n^5 + 20*n^4 + (68/3)*n^3 + 12*n^2 + (7/3)*n + 1
 *            Equivalent integer form: a(n) = (3*n^6 + 27*n^5 + 60*n^4 +
 *            68*n^3 + 36*n^2 + 7*n + 3)/3 (division is exact for all n ≥ 1.)
 *            Computes the term for any n ≥ 1, using only integer arithmetic
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 37, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Promote the index to sequence type once */
    seq_t N = (seq_t)n;

    /* Compute successive powers of n */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */
    seq_t n5 = n4 * N;         /* n^5 */
    seq_t n6 = n5 * N;         /* n^6 */

    /* Build the numerator: 3*n^6 + 27*n^5 + 60*n^4 + 68*n^3 + 36*n^2 + 7*n + 3 */
    seq_t numer = 3 * n6
                + 27 * n5
                + 60 * n4
                + 68 * n3
                + 36 * n2
                + 7  * N
                + 3;

    /* Exact division by 3 gives the required term */
    return numer / 3;
}
