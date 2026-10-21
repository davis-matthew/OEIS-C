/*
 * A256328  Number of partitions of 6n into exactly 4 parts.
 * https://oeis.org/A256328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1+(-1)^n+6*n^2+12*n^3)/8
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t n2  = n64 * n64;
    uint64_t n3  = n2 * n64;
    uint64_t val = 12ULL * n3 + 6ULL * n2;
    if (n & 1ULL) val -= 2ULL;
    return (seq_t)(val / 8ULL);
}
