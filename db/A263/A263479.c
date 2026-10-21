/*
 * A263479  Total number of n-digit positive integers with multiplicative
 *          digital root value 5.
 * https://oeis.org/A263479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/720)*(3*n^8 + 6*n^7 - 664*n^6 + 6270*n^5 - 25783*n^4 +
 *            55164*n^3 - 57796*n^2 + 23520*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;           /* n^2 */
    seq_t n3 = n2 * ns;           /* n^3 */
    seq_t n4 = n3 * ns;           /* n^4 */
    seq_t n5 = n4 * ns;           /* n^5 */
    seq_t n6 = n5 * ns;           /* n^6 */
    seq_t n7 = n6 * ns;           /* n^7 */
    seq_t n8 = n7 * ns;           /* n^8 */

    /* Positive contributions */
    seq_t pos = (seq_t)0;
    pos += (seq_t)3 * n8;
    pos += (seq_t)6 * n7;
    pos += (seq_t)6270 * n5;
    pos += (seq_t)55164 * n3;
    pos += (seq_t)23520 * ns;

    /* Negative contributions (subtracted later) */
    seq_t neg = (seq_t)0;
    neg += (seq_t)664 * n6;
    neg += (seq_t)25783 * n4;
    neg += (seq_t)57796 * n2;

    seq_t res = pos - neg;
    res /= (seq_t)720;
    return res;
}
