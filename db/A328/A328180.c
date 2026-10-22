/*
 * A328180  a(n) is the maximum number of 5-cycles possible in an n-vertex
 *          planar graph.
 * https://oeis.org/A328180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n<5; a(5)=6; a(6)=24; a(7)=41; a(n)=2*n^2-10*n+12
 *            for n>=8
 * Range:     u8 n <= 13, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    if (n == 5)
        return 6;
    if (n == 6)
        return 24;
    if (n == 7)
        return 41;
    /* n >= 8 */
    uint64_t nt = n;
    uint64_t val = 2ULL * nt * nt - 10ULL * nt + 12ULL;
    return (seq_t)val;
}
