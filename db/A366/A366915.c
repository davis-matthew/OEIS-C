/*
 * A366915  a(n) = Sum_{k=1..n} (-1)^k*k^2*floor(n/k).
 * https://oeis.org/A366915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^k * k^2 * floor(n/k)
 * Range:     i8 n <= 14, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k * k;          /* k^2 as seq_t */
        seq_t fl = (seq_t)(n / k);        /* floor(n/k) as seq_t */
        if (k & 1)                       /* k is odd -> sign -1 */
            sum -= kk * fl;
        else
            sum += kk * fl;               /* k is even -> sign +1 */
    }
    return sum;
}
