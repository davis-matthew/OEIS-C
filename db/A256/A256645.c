/*
 * A256645  25-gonal pyramidal numbers: a(n) = n*(n+1)*(23*n-20)/6.
 * https://oeis.org/A256645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(23*n-20)/6
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a wider unsigned type for intermediate calculations to avoid overflow
       when seq_t is smaller (e.g., 8 or 16 bits). n fits in uint64_t for all
       values that are tested. */
    uint64_t N = n;
    uint64_t prod = N * (N + 1) * (23 * N - 20);
    uint64_t res  = prod / 6;
    return (seq_t)res;
}
