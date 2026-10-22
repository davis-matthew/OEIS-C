/*
 * A344404  a(n) = Sum_{d|n} floor(n/d^2).
 * https://oeis.org/A344404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} floor(n/d^2)
 * Range:     u8 n <= 15, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    seq_t N = (seq_t)n;            /* cast the index to the return type */
    seq_t sum = 0;

    /* iterate d from 1 to √n */
    for (idx_t d = 1; (seq_t)d * d <= N; ++d)
    {
        if (n % d == 0)
        {
            seq_t dd = (seq_t)d * d;          /* d²  */
            sum += N / dd;                   /* floor(n/d²) */

            idx_t other = n / d;               /* the paired divisor n/d */
            if (other != d)
            {
                seq_t ddd = (seq_t)other * other;
                sum += N / ddd;               /* floor(n/(n/d)²) */
            }
        }
    }

    return sum;
}
