/*
 * A261309  a(n+1) = abs(a(n) - gcd(a(n), 9n+8)), u(1) = 1.
 * https://oeis.org/A261309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* a(1) = 1
   a(n+1) = |a(n) – gcd(a(n), 9n+8)|   for n ≥ 1
*/

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* offset is 1 */
    if (n == 1) return (seq_t)1;            /* a(1) = 1 */

    seq_t prev = (seq_t)1;                  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* compute 9*(i-1)+8 */
        seq_t val = (seq_t)(i - 1);
        val = val * (seq_t)9 + (seq_t)8;

        seq_t g = gcd(prev, val);
        seq_t diff = (prev > g) ? (prev - g) : (g - prev);
        prev = diff;
    }
    return prev;
}
