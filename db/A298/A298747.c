/*
 * A298747  If n = 2^(2*m+1)+j, 0 <= j < 3*2^(2*m+1), then a(n) = 2^(3*m)+j.
 * https://oeis.org/A298747
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3*m) + (n - 2^(2*m+1)) where n = 2^(2*m+1) + j, 0 <= j
 *            < 3*2^(2*m+1). This function finds the correct block for n and
 *            returns the value directly
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined for offset < 2 */

    /* compute floor(log2(n)) in a loop (works for idx_t = uint64_t) */
    idx_t l = 0;
    idx_t tmp = n;
    while (tmp > 1) { tmp >>= 1; ++l; }

    /* make exponent odd: k = 2*m+1 */
    if ((l & 1) == 0) --l;              /* if l even, use previous odd */
    idx_t k = l;                        /* k is odd, k = 2*m+1 */

    /* compute base values */
    seq_t base_power = pow2(k);         /* 2^k */
    seq_t offset = (seq_t)n - base_power;  /* j = n - 2^k */

    idx_t m = (k - 1) / 2;              /* m = (k-1)/2 */
    seq_t base_val = pow2(3 * m);      /* 2^(3*m) */

    return base_val + offset;
}
