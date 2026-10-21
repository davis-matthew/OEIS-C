/*
 * A255270  Integer part of fourth root of n.
 * https://oeis.org/A255270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(1/4))
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t) n;          /* convert index to seq_t */
    seq_t r  = floor_sqrt(sn);     /* floor(sqrt(n)) */
    return floor_sqrt(r);          /* floor(sqrt(floor(sqrt(n)))) = floor(n^(1/4)) */
}
