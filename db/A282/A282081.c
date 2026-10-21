/*
 * A282081  Number of n-element subsets of [n+5] having an even sum.
 * https://oeis.org/A282081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)(n+2)(n+3)(n+4)(n+5) + ((-i)^n + i^n)(8+6n+n^2)/15)
 *            / 240 Simplified: a(n) = (prod + e*15*C) / 240 where prod =
 *            (n+1)(n+2)(n+3)(n+4)(n+5), C = 8 + 6n + n^2, e = 1 if n ≡ 0 (mod
 *            4), -1 if n ≡ 2 (mod 4), 0 otherwise
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 249, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate values.
       No conversion from seq_t to __int128 is performed. */
    unsigned __int128 N = n;

    /* prod = (n+1)(n+2)(n+3)(n+4)(n+5) */
    unsigned __int128 prod = 1;
    prod *= (N + 1);
    prod *= (N + 2);
    prod *= (N + 3);
    prod *= (N + 4);
    prod *= (N + 5);

    /* C = 8 + 6n + n^2 */
    unsigned __int128 C = 8 + 6 * N + N * N;

    /* 15*C */
    unsigned __int128 fifteenthC = 15 * C;

    /* Determine e according to n mod 4 */
    int mod4 = n & 3;
    unsigned __int128 numerator;
    if (mod4 == 0)         numerator = prod + fifteenthC;
    else if (mod4 == 2)    numerator = prod - fifteenthC;
    else                   numerator = prod;

    /* Exact division by 240 (240 = 15 * 16) */
    unsigned __int128 result = numerator / 240;

    return (seq_t)result;
}
