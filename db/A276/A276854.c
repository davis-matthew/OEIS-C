/*
 * A276854  Beatty sequence for 1 + sqrt(5).
 * https://oeis.org/A276854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1 + sqrt(5))) = n + floor(sqrt(5*n*n))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t prod = n;
    prod *= n;       // prod = n * n
    prod *= 5;       // prod = 5 * n * n
    seq_t s = floor_sqrt(prod);
    return n + s;
}
