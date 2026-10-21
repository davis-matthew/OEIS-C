/*
 * A269659  Number of length-6 0..n arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 15*n^4 + 14*n^3 + 3*n^2 + 3*n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; a(0) is unused but safe */
    seq_t N  = (seq_t)n;          /* cast first to ensure seq_t arithmetic */
    seq_t n2 = N * N;             /* n^2 */
    seq_t n3 = n2 * N;            /* n^3 */
    seq_t n4 = n3 * N;            /* n^4 */
    seq_t n5 = n4 * N;            /* n^5 */
    seq_t n6 = n5 * N;            /* n^6 */

    seq_t res;
    res = n6;
    res += (seq_t)6 * n5;
    res += (seq_t)15 * n4;
    res += (seq_t)14 * n3;
    res += (seq_t)3 * n2;
    res += (seq_t)3 * N;

    return res;
}
