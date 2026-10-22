/*
 * A307044  a(n) = Sum_{k=0..floor(n/8)} (-1)^k*binomial(n,8*k).
 * https://oeis.org/A307044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/8⌋} (-1)^k * binomial(n,8k) Recurrence (for
 *            n>7): a(0..7) = 1 a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) -
 *            70*a(n-4) + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - 2*a(n-8)
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 34, i64 n <= 67, i128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 7)
        return 1;

    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = 1;

    for (idx_t k = 8; k <= n; ++k)
    {
        seq_t a1 = buf[(k - 1) & 7];
        seq_t a2 = buf[(k - 2) & 7];
        seq_t a3 = buf[(k - 3) & 7];
        seq_t a4 = buf[(k - 4) & 7];
        seq_t a5 = buf[(k - 5) & 7];
        seq_t a6 = buf[(k - 6) & 7];
        seq_t a7 = buf[(k - 7) & 7];
        seq_t a8 = buf[(k - 8) & 7];

        seq_t val =
            8 * a1
            - 28 * a2
            + 56 * a3
            - 70 * a4
            + 56 * a5
            - 28 * a6
            + 8  * a7
            - 2  * a8;

        buf[k & 7] = val;
    }
    return buf[n & 7];
}
