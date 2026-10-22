/*
 * A303767  May code of n: a(0) = 0, and for n > 0, if n = 2^k, a(n) = n +
 *          a(n-1), otherwise, when n = 2^k + r (with 0 < r < 2^k), then a(n)
 *          = 2^k + a(r-1); see comments for equivalent alternative
 *          descriptions.
 * https://oeis.org/A303767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; For n > 0, let p = 2^⌊log₂n⌋. If n = p, a(n) = n + a(n
 *            – 1); else (n = p + r, 1 ≤ r < p), a(n) = p + a(n – p – 1)
 * Range:     u8 n <= 255, u16 n <= 9546, u32 n <= 9546, u64 n <= 9546,
 *            u128 n <= 9546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    // find largest power of two <= n
    idx_t p = 1;
    while (p <= n / 2) p <<= 1;   // safe from overflow

    if (p == n) /* power of two */
        return (seq_t)n + a(n - 1);
    else        /* n = p + r, 1 <= r < p */
        return (seq_t)p + a(n - p - 1);
}
