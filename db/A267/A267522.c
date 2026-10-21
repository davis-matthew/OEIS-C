/*
 * A267522  a(n) = 4*(n + 1)*(n + 2)*(4*n + 3)/3.
 * https://oeis.org/A267522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n + 1)*(n + 2)*(4*n + 3)/3
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = n;                           /* n is unsigned 64‑bit */
    /* use a wider type to avoid intermediate overflow */
    unsigned __int128 prod = (unsigned __int128)(N + 1);
    prod *= (unsigned __int128)(N + 2);
    prod *= (unsigned __int128)(4 * N + 3);
    prod *= 4;
    prod /= 3;
    return (seq_t)prod;
}
