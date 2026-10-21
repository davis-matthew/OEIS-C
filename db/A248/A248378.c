/*
 * A248378  a(2n) = concatenation of n+1 with n+2, a(2n+1) = concatenation of
 *          n+2 with n+1.
 * https://oeis.org/A248378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2n) = concat(n+1, n+2); a(2n+1) = concat(n+2, n+1)
 * Range:     u8 n <= 15, u16 n <= 195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/concat_parts.h"

seq_t a(idx_t n) {
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                /* k = n/2 */
        return concat_parts(k + 1, k + 2);
    } else {
        idx_t k = n >> 1;                /* k = (n-1)/2 */
        return concat_parts(k + 2, k + 1);
    }
}
