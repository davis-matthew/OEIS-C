/*
 * A252822  Number of n X 2 nonnegative integer arrays with upper left 0 and
 *          every value within 3 of its city block distance from the upper
 *          left and every value increasing by 0 or 1 with every step right or
 *          down.
 * https://oeis.org/A252822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 117, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A252822.
   Formula (recurrence):
      a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3)
             - 35*a(n-4) + 21*a(n-5) - 7*a(n-6) + a(n-7)
   for n > 7.
   Initial values (offset 1):
      a(1)=2, a(2)=6, a(3)=18, a(4)=53, a(5)=142,
      a(6)=339, a(7)=729. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[7] = {2, 6, 18, 53, 142, 339, 729};

    if (n >= 1 && n <= 7)
        return init[n - 1];

    /* Circular buffer of last seven terms:
       prev[0] = a(cur-7), prev[6] = a(cur-1)   */
    seq_t prev[7];
    for (size_t i = 0; i < 7; ++i)
        prev[i] = init[i];

    seq_t result = 0;
    for (idx_t cur = 8; cur <= n; ++cur)
    {
        /* a(cur) = 7*a(cur-1) + 35*a(cur-3) + 21*a(cur-5) + a(cur-7)
                      - 21*a(cur-2) - 35*a(cur-4) - 7*a(cur-6) */
        seq_t sumPos = prev[6] * 7 + prev[4] * 35 + prev[2] * 21 + prev[0];
        seq_t sumNeg = prev[5] * 21 + prev[3] * 35 + prev[1] * 7;
        result = sumPos - sumNeg;  /* guaranteed non‑negative */

        /* rotate buffer: drop a(cur-7), shift left, insert a(cur) */
        for (size_t j = 0; j < 6; ++j)
            prev[j] = prev[j + 1];
        prev[6] = result;
    }
    return result;
}
