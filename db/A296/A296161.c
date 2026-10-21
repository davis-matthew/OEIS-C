/*
 * A296161  Sum of the larger parts of the partitions of n into two parts such
 *          that the smaller part is odd.
 * https://oeis.org/A296161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n-i) * (i mod 2)
 * Range:     u8 n <= 37, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n) {
    /* m = floor(n/2) */
    idx_t m = n >> 1;
    /* k = ceil(m/2) = (m+1)/2 */
    idx_t k = (m + 1) >> 1;
    seq_t sn = (seq_t)n;
    seq_t kk = (seq_t)k;
    return kk * sn - kk * kk;
}
