/*
 * A368624  a(n) = Sum_{k = 1..n} (-1)^(n+k) * gcd(2*k, n).
 * https://oeis.org/A368624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 1..n} (-1)^(n+k) * gcd(2*k, n)
 * Range:     u8 n <= 63, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* below offset, arbitrary */
    if (n & 1) return (seq_t)n;          /* odd n: a(n) = n */

    idx_t half = n >> 1;                 /* n = 2*half */
    if (half & 1) return 0;              /* n = 4*k+2: a(n)=0 */

    /* n is divisible by 4: n = 4*m */
    idx_t m = n >> 2;                    /* m = n/4 */
    seq_t sum = 0;

    for (idx_t k = 1; k <= m; ++k) {
        seq_t g = gcd((seq_t)(2 * k), (seq_t)m);
        sum += g;
    }
    return sum * 4;
}
