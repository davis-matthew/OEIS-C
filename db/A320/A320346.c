/*
 * A320346  a(n) is the number of perfect matchings in the graph with vertices
 *          labeled 1 to 2n with edges {i,j} for 1<=|i-j|<=4.
 * https://oeis.org/A320346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 6*a(n-3) + 3*a(n-4) + 2*a(n-5) +
 *            a(n-6) * - 2*a(n-7) - a(n-8) (n ≥ 8) * Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=12, a(4)=35, * a(5)=105, a(6)=329,
 *            a(7)=1014
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 8 initial terms of the sequence */
    static const seq_t init[8] = {1, 1, 3, 12, 35, 105, 329, 1014};

    /* Return the initial value if n < 8 */
    if (n < 8)
        return init[(size_t)n];

    /* buffer holds the last 8 computed values,
       where buf[0] = a(i-8), buf[7] = a(i-1) before step i */
    seq_t buf[8];
    for (size_t k = 0; k < 8; ++k)
        buf[k] = init[k];

    /* Compute a(n) iteratively */
    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t next = 2 * buf[7]      /* 2*a(i-1) */
                  +      buf[6]      /* a(i-2)   */
                  + 6 * buf[5]      /* 6*a(i-3) */
                  + 3 * buf[4]      /* 3*a(i-4) */
                  + 2 * buf[3]      /* 2*a(i-5) */
                  +      buf[2]      /* a(i-6)   */
                  - 2 * buf[1]      /* -2*a(i-7)*/
                  -      buf[0];     /* -a(i-8)  */

        /* Shift buffer: discard oldest value, make room for next */
        for (size_t j = 0; j < 7; ++j)
            buf[j] = buf[j + 1];
        buf[7] = next;   /* buf[7] now holds a(i) */
    }

    return buf[7];
}
