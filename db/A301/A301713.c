/*
 * A301713  Partial sums of A301712.
 * https://oeis.org/A301713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the linear recurrence * a(n) = 2*a(n-1) -
 *            a(n-2) - a(n-4) + 3*a(n-5) - 3*a(n-6) + * a(n-7) + a(n-9) -
 *            2*a(n-10) + a(n-11) for n > 10 * Initial terms: * a(0) = 1, a(1)
 *            = 6, a(2) = 16, a(3) = 32, a(4) = 54, * a(5) = 81, a(6) = 114,
 *            a(7) = 152, a(8) = 195, a(9) = 244, * a(10) = 29...
 * Range:     u8 n <= 9, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[11] = {
        (seq_t)1,
        (seq_t)6,
        (seq_t)16,
        (seq_t)32,
        (seq_t)54,
        (seq_t)81,
        (seq_t)114,
        (seq_t)152,
        (seq_t)195,
        (seq_t)244,
        (seq_t)297
    };

    if (n < 11)
        return base[(size_t)n];

    /* Keep the last 12 computed values in a circular buffer. */
    seq_t buf[12];
    for (size_t i = 0; i < 11; ++i)
        buf[i] = base[i];

    seq_t next;
    for (idx_t i = 11; i <= n; ++i)
    {
        /* Indices relative to i: (i - k) % 12 */
        size_t i1 = (size_t)(i - 1) % 12;
        size_t i2 = (size_t)(i - 2) % 12;
        size_t i4 = (size_t)(i - 4) % 12;
        size_t i5 = (size_t)(i - 5) % 12;
        size_t i6 = (size_t)(i - 6) % 12;
        size_t i7 = (size_t)(i - 7) % 12;
        size_t i9 = (size_t)(i - 9) % 12;
        size_t i10 = (size_t)(i - 10) % 12;
        size_t i11 = (size_t)(i - 11) % 12;

        next = (seq_t)2 * buf[i1]
             - buf[i2]
             - buf[i4]
             + (seq_t)3 * buf[i5]
             - (seq_t)3 * buf[i6]
             + buf[i7]
             + buf[i9]
             - (seq_t)2 * buf[i10]
             + buf[i11];

        buf[(size_t)i % 12] = next;
    }

    return buf[(size_t)n % 12];
}
