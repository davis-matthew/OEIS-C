/*
 * A373329  a(n)^2 is the greatest square not exceeding A000217(n^2).
 * https://oeis.org/A373329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( n² * (n² + 1) / 2 ) )
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 52, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t val = nsq * (nsq + 1) / 2;
    return floor_sqrt(val);
}
