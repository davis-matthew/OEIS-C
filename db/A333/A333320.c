/*
 * A333320  a(n) is the number of subsets of {1..n} that contain exactly 4 odd
 *          and 1 even numbers.
 * https://oeis.org/A333320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(ceil(n/2),4) * floor(n/2)
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 320, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t odd   = (n + 1) >> 1;    /* ceil(n/2) */
    idx_t even  = n >> 1;          /* floor(n/2) */
    seq_t bin   = binomial(odd, 4);
    return bin * (seq_t)even;
}
