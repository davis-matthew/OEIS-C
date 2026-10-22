/*
 * A302291  a(n) is the period of the binary expansion of n.
 * https://oeis.org/A302291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the period of the binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* special case */
        return (seq_t)1;

    /* bit length of n */
    idx_t k = 0;
    idx_t temp = n;
    while (temp) { k++; temp >>= 1; }

    /* d = 2^k - 1 */
    uint64_t d;
    if (k == 64)
        d = UINT64_MAX;           /* 2^64 - 1 fits in 64 bits */
    else
        d = ((uint64_t)1 << k) - 1;

    /* compute gcd(n, d) */
    idx_t a1 = n, b1 = d;
    while (b1 != 0) {
        idx_t r = a1 % b1;
        a1 = b1;
        b1 = r;
    }
    uint64_t g = a1;

    /* reduced denominator */
    uint64_t m = d / g;
    if (m == 1)
        return (seq_t)1;          /* integer results have period 1 */

    /* multiplicative order of 2 modulo m */
    uint64_t x = 1;
    uint64_t period = 0;
    do {
        x = (x * 2) % m;
        period++;
    } while (x != 1);

    return (seq_t)period;
}
