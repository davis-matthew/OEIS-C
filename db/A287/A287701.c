/*
 * A287701  a(n) = (5!)^3 * [z^5] hypergeom([], [1,1], z)^n.
 * https://oeis.org/A287701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 163476*n - 375375*n^2 + 305500*n^3 - 108000*n^4 +
 *            14400*n^5 (derived from the OGF
 *            x*(1+2246x+98256x^2+660746x^3+966751x^4)/(1-x)^6)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit signed integer to keep intermediate values exact,
       then cast the final result to seq_t. */
    __int128 nn = n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;

    __int128 res = 14400 * n5;
    res -= 108000 * n4;
    res += 305500 * n3;
    res -= 375375 * n2;
    res += 163476 * nn;

    return (seq_t)res;
}
