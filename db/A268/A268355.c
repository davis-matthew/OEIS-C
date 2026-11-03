/*
 * A268355  Highest power of 8 dividing n.
 * https://oeis.org/A268355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 8^valuation(n,8) = 8^floor(v2(n)/3)
 * Range:     u8 n <= 511, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 103 terms through n = 9742 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

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
    return ipow_u64_idx(8, p);      /* 8^p as seq_t */
}
