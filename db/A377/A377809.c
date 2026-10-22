/*
 * A377809  k*(k+3)/2 appears k times.
 * https://oeis.org/A377809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = k*(k+3)/2 where k is the smallest integer with k*(k+1)/2
 *            >= n. That is, the number k*(k+3)/2 appears k times in the
 *            sequence
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1; undefined below */

    idx_t k = 1;                          /* current block index */
    idx_t cumulative = 1;                 /* cumulative count up to this block */

    /* find the block that contains the n‑th term */
    while (n > cumulative) {
        ++k;
        cumulative += k;
    }

    seq_t k_s = (seq_t)k;
    return k_s * (k_s + 3) / 2;
}
