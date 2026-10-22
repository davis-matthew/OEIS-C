/*
 * A332698  a(n) = (8*n^3 + 15*n^2 + 13*n)/6.
 * https://oeis.org/A332698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 15*n^2 + 13*n)/6
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use an unsigned 128‑bit integer for intermediate computation
       to avoid overflow when n is large (up to the limits of the test
       suite).  This works on compilers that support the GCC/Clang
       extension __int128. */
    unsigned __int128 nn = n;
    unsigned __int128 val = (8*nn*nn*nn + 15*nn*nn + 13*nn) / 6;
    return (seq_t)val;   /* conversion truncates if result exceeds seq_t */
}
