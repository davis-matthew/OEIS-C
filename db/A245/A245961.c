/*
 * A245961  Number of 4-cycles in the Lucas cube Lambda(n).
 * https://oeis.org/A245961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 80, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=5 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[6] = {0, 0, 0, 0, 2, 5};

    if (n < 6)
        return init[n];

    /* successive values are kept in a sliding window of length 6:
       a0 = a(n-6), a1 = a(n-5), a2 = a(n-4), a3 = a(n-3),
       a4 = a(n-2), a5 = a(n-1)  */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];

    for (idx_t i = 6; i <= n; ++i)
    {
        /* a(n) = 3*a(n-1) + 3*a(n-5) + a(n-6) - 5*a(n-3) */
        seq_t val = 3 * a5 + 3 * a1 + a0 - 5 * a3;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = val;
    }

    return a5;
}
