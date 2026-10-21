/*
 * A254279  Decimal expansion of deuteron mass in kg.
 * https://oeis.org/A254279
 *
 * Offset:    -26    (a(-26) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= -20, u16 n <= -20, u32 n <= -20, u64 n <= -20,
 *            u128 n <= -20, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254279: decimal digits of the deuteron mass 3.34358348 × 10⁻²⁷ kg.
   The mantissa 3 3 4 3 5 8 3 4 8 occupies indices -26 … -18; other indices
   return 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t digits[9] = {3, 3, 4, 3, 5, 8, 3, 4, 8};

    if (n < -26)                /* undefined part */
        return 0;

    idx_t pos = n + 26;          /* 0 for first digit */

    if (pos < 9)                 /* within mantissa */
        return digits[(size_t)pos];

    return 0;                    /* trailing zeros */
}
