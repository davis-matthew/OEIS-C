/*
 * A301690  Coordination sequence for node of type V1 in "krf" 2-D tiling (or
 *          net).
 * https://oeis.org/A301690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(0)=1, a(1)=6, a(2)=6,
 *            a(3)=9, a(4)=12, a(5)=18, a(6)=24, a(7)=21, a(8)=24, a(9)=30,
 *            a(10)=36; For n ≥ 11: a(n) = 2*a(n−5) – a(n−10)
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[10] =
        { (seq_t)1, (seq_t)6, (seq_t)6, (seq_t)9, (seq_t)12,
          (seq_t)18, (seq_t)24, (seq_t)21, (seq_t)24, (seq_t)30 };

    if (n < 10)
        return init[(size_t)n];

    if (n == 10)
        return (seq_t)36;

    /* Circular buffer holding the last 10 terms.
       After the initial copy, buffer[i] = a(i) for i=0..9. */
    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i)
        buf[i] = init[i];

    /* Compute a(10) explicitly and shift the buffer. */
    seq_t cur = (seq_t)36;
    for (size_t i = 0; i < 9; ++i)
        buf[i] = buf[i + 1];
    buf[9] = cur;              /* buf now holds a(1)..a(10) */

    /* Generate terms up to a(n). */
    for (idx_t k = 11; k <= n; ++k)
    {
        /* a(k) = 2*a(k-5) - a(k-10) using the buffer. */
        cur = (seq_t)2 * buf[5] - buf[0];

        /* Shift buffer left by one and insert cur at the end. */
        for (size_t i = 0; i < 9; ++i)
            buf[i] = buf[i + 1];
        buf[9] = cur;
    }

    return buf[9];
}
