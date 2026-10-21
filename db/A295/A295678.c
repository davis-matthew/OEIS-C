/*
 * A295678  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 2, a(2) =
 *          1, a(3) = 3.
 * https://oeis.org/A295678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) Initial values: a(0) = 1, a(1) =
 *            2, a(2) = 1, a(3) = 3
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* Circular buffer that keeps the last five computed values
       (a(n-4), a(n-3), a(n-2), a(n-1), a(n)) */
    seq_t buf[5] = { (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)3, 0 };

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t val = buf[(size_t)(i - 1) % 5]   /* a(i-1) */
                  + buf[(size_t)(i - 3) % 5]   /* a(i-3) */
                  + buf[(size_t)(i - 4) % 5];  /* a(i-4) */
        buf[(size_t)i % 5] = val;              /* store a(i) */
    }

    return buf[(size_t)(n % 5)];
}
