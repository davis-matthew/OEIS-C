/*
 * A245235  Repeat 2^(n*(n+1)/2) n+1 times.
 * https://oeis.org/A245235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(t) where t = floor((sqrt(8*n+1)-1)/2) *
 *            (floor((sqrt(8*n+1)-1)/2)+1)/2 Equivalent: let m =
 *            floor((sqrt(8*n+1)-1)/2); a(n) = 2^(m*(m+1)/2)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the block index m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    while (((m + 1) * (m + 2)) / 2 <= n) {
        ++m;
    }
    /* Exponent is the triangular number t = m*(m+1)/2 */
    idx_t t = m * (m + 1) / 2;
    return pow2(t);
}
