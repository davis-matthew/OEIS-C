/*
 * A261791  The integer part of the surface area of the 4-dimensional sphere
 *          of radius n.
 * https://oeis.org/A261791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*Pi^2*n^3)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2*Pi^2 ≈ 19.739208802178716
       We use the rational approximation P/Q with Q = 10^17
       P = round(2*Pi^2 * 10^17) = 1973920880217871600 */
    static const unsigned __int128 P = (__int128)1973920880217871600ULL;
    static const unsigned __int128 Q = (__int128)100000000000000000ULL;

    if (n == 0) return 0;  /* safety for offsets below 1 */

    /* compute n^3 using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 n3 = (unsigned __int128)n * n * n;

    /* floor( P/Q * n^3 ) = floor( (P * n^3) / Q ) */
    unsigned __int128 prod = P * n3;
    unsigned __int128 val  = prod / Q;

    return (seq_t)val;
}
