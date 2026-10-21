/*
 * A268676  a(n) = A101080(n,A268823(3+n)), where A101080(x,y) gives the
 *          Hamming distance between binary expansions of x and y.
 * https://oeis.org/A268676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 - (-1)^A000120(n) = 1 + 2*A010060(n)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* parity of the popcount of n */
    uint64_t v = (uint64_t)n;
    v ^= v >> 32;
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    unsigned parity = v & 1U;
    return (seq_t)(1 + 2 * parity);
}
