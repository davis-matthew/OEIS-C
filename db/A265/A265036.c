/*
 * A265036  Coordination sequence of 2-uniform tiling {3.4.6.4, 4.6.12} with
 *          respect to a point of type 3.4.6.4.
 * https://oeis.org/A265036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 4*a(n-1) - 8*a(n-2)
 *            + 10*a(n-3) - 8*a(n-4) + 4*a(n-5) - a(n-6) for n ≥ 10, with the
 *            first ten terms a(0)=1, a(1)=4, a(2)=6, a(3)=7, a(4)=10,
 *            a(5)=14, a(6)=20, a(7)=24, a(8)=24, a(9)=23. The recurrence is
 *            applied using only additions and subtractions tha...
 * Range:     u8 n <= 71, u16 n <= 9146, u32 n <= 9146, u64 n <= 9146,
 *            u128 n <= 9146, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first ten terms of the sequence */
    static const seq_t init[10] = {
        1, 4, 6, 7, 10, 14, 20, 24, 24, 23
    };

    if (n < 10)
        return init[(size_t)n];

    /* use a circular buffer of length 10 to keep the last 10 terms */
    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i)
        buf[i] = init[i];

    /* iterate from 10 up to n, computing each new term */
    for (idx_t i = 10; i <= n; ++i) {
        /* indices relative to current buf state:
           buf[9] = a(i-1)
           buf[8] = a(i-2)
           buf[7] = a(i-3)
           buf[6] = a(i-4)
           buf[5] = a(i-5)
           buf[4] = a(i-6)  */
        seq_t pos = 4 * buf[9] + 10 * buf[7] + 4 * buf[5];
        seq_t neg = 8 * buf[8] + 8 * buf[6] + buf[4];
        seq_t next = pos - neg;        /* pos ≥ neg for all n ≥ 10 */

        /* shift buffer left by one position */
        for (int j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = next;
    }

    /* after the loop, buf[9] holds a(n) */
    return buf[9];
}
