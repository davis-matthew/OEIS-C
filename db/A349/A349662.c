/*
 * A349662  a(n) is the number of squares strictly between n^2 and n^3.
 * https://oeis.org/A349662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0 or 1; a(n) = floor( sqrt(n^3 - 1) ) - n for n
 *            > 1
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 992, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t t  = n3 - 1;
    seq_t s  = floor_sqrt(t);
    return s - ns;
}
