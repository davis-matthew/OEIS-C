/*
 * A268355  Highest power of 8 dividing n.
 * https://oeis.org/A268355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 8^valuation(n,8) = 8^floor(v2(n)/3)
 * Range:     u8 n <= 511, u16 n <= 9742, u32 n <= 9742, u64 n <= 9742,
 *            u128 n <= 9742, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; this case is not used, but safe */
        return 0;

    unsigned int e2 = 0;   /* exponent of 2 in n */
    idx_t m = n;

    /* count trailing zero bits of n */
    while ((m & 1) == 0) {
        ++e2;
        m >>= 1;
    }

    unsigned int p = e2 / 3;    /* highest exponent of 8 dividing n */
    return pow_uint(8, p);      /* 8^p as seq_t */
}
