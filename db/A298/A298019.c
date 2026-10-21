/*
 * A298019  Partial sums of A298016.
 * https://oeis.org/A298019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7), a(0) = 1,
 *            a(1) = 7, a(2) = 19, a(3) = 31, a(4) = 55, a(5) = 91, a(6) = 115
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..6 */
    static const seq_t init[7] = { 1, 7, 19, 31, 55, 91, 115 };

    if (n <= 6)
        return init[n];

    /* Sliding window of the last seven values.
       val[0] holds a(n-7), val[6] holds a(n). */
    seq_t val[7];
    for (size_t i = 0; i < 7; ++i)
        val[i] = init[i];

    /* Compute values from a(7) up to a(n). */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = val[6];            /* a(n-1)   */
        next += val[4]; next += val[4]; /* +2*a(n-3)*/
        next -= val[3]; next -= val[3]; /* -2*a(n-4)*/
        next -= val[1];                 /* -a(n-6)  */
        next += val[0];                 /* +a(n-7)  */

        /* Shift the window: discard a(n-7),
           move others left, store new a(n). */
        for (size_t j = 0; j < 6; ++j)
            val[j] = val[j + 1];
        val[6] = next;
    }

    return val[6];
}
