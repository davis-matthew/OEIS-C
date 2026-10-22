/*
 * A332150  a(n) = 5*(10^(2*n+1) - 1)/9 - 5*10^n.
 * https://oeis.org/A332150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 - 5*10^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The same function works for 8/16/32/64-bit and arbitrary precision
   seq_t types.  The computation uses a 128‑bit intermediate to avoid
   overflow for the range of n that yields values fitting in seq_t. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Build the integer consisting of (2*n+1) 1's:
       111…1  (2*n+1 digits)
    */
    unsigned __int128 ones = 0;
    for (idx_t i = 0; i <= 2*n; ++i) {
        ones = ones * 10 + 1;
    }

    /* Multiply by 5 to get 555…5 */
    unsigned __int128 val = ones * 5;

    /* Subtract 5*10^n */
    unsigned __int128 pow10n = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow10n *= 10;
    }
    val -= pow10n * 5;

    return (seq_t)val;
}
