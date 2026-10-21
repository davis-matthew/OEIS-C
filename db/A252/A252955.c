/*
 * A252955  Number of (n+2)X(2+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 8*a(n-3) - 55*a(n-4) + 75*a(n-5) +
 *            71*a(n-6) - 85*a(n-7) - 42*a(n-8) - 600*a(n-9) + 1575*a(n-10) -
 *            1323*a(n-11) + 378*a(n-12) a(1) = 119, a(2) = 120, a(3) = 185,
 *            a(4) = 337, a(5) = 596, a(6) = 1186, a(7) = 2279, a(8) = 4165,
 *            a(9) = 8552, a(10) = 16744, a(11) = 31079,...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 28, u64 n <= 59, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 14 terms */
    static const seq_t init[14] = {
        119, 120, 185, 337, 596, 1186,
        2279, 4165, 8552, 16744, 31079,
        64381, 127298, 237724
    };

    /* recurrence coefficients, 12 terms */
    static const int COEF[12] = {
        5, -6, 8, -55, 75, 71,
        -85, -42, -600, 1575, -1323, 378
    };

    if (n <= 14)  /* direct lookup for the first 14 terms */
    {
        return init[n - 1];
    }

    /* prev[0] = a(k-1), prev[1] = a(k-2), ..., prev[11] = a(k-12)  */
    seq_t prev[12];
    for (size_t i = 0; i < 12; ++i)
    {
        prev[i] = init[13 - i];   /* init[13] = a14, init[12] = a13, … , init[2] = a3 */
    }

    /* compute up to a(n) */
    for (idx_t k = 15; k <= n; ++k)
    {
        seq_t sum = 0;   /* use seq_t for arbitrary-precision arithmetic */
        for (size_t i = 0; i < 12; ++i)
        {
            int c = COEF[i];
            seq_t term = prev[i];
            if (c >= 0)
            {
                sum += term * (seq_t)c;
            }
            else
            {
                sum -= term * (seq_t)(-c);
            }
        }
        seq_t next = sum; /* the new value a(k) */

        /* shift the window: drop the oldest term, make room for next */
        for (size_t i = 11; i > 0; --i)
        {
            prev[i] = prev[i - 1];
        }
        prev[0] = next;
    }

    return prev[0];
}
