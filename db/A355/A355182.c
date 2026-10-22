/*
 * A355182  a(n) = t(n) - s(n) where s(n) = n*(n-1)/2 is the sum of the first
 *          n nonnegative integers and t(n) is the smallest sum of consecutive
 *          integers starting from n such that t(n) > s(n).
 * https://oeis.org/A355182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = t(n) - s(n) * where * s(n) = n*(n-1)/2 * j = ⌊√(8*n² −
 *            8*n + 1)⌋ * m = ⌈j/2⌉ − n + 1 * t(n) = (m*(m + 2*n − 1))/2
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;

    /* s(n) = n*(n-1)/2 */
    seq_t s = sn * (sn - (seq_t)1) / (seq_t)2;

    /* val = 8*n² - 8*n + 1 */
    seq_t val = ((seq_t)8 * sn * sn) - ((seq_t)8 * sn) + (seq_t)1;

    /* j = floor_sqrt(val) */
    seq_t j = floor_sqrt(val);

    /* halfCeil = ceil(j/2) = (j+1)/2 */
    seq_t halfCeil = (j + (seq_t)1) / (seq_t)2;

    /* m = halfCeil - n + 1 */
    seq_t m = halfCeil - sn + (seq_t)1;

    /* t(n) = m*(m + 2*n - 1)/2 */
    seq_t t = m * (m + (seq_t)2 * sn - (seq_t)1) / (seq_t)2;

    return t - s;
}
