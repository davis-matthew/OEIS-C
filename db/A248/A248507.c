/*
 * A248507  Decimal expansion of a single magnetic flux quantum Phi_0 in SI
 *          units.
 * https://oeis.org/A248507
 *
 * Offset:    -14    (a(-14) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit (index n+14) of the rational number
 *            3313035075/1602176634, which equals the magnetic flux quantum Φ₀
 *            = h/(2e). The sequence is defined for n ≥ –14 with a(–14) = 2
 *            being the first digit of the significand
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < -14)  /* undefined below offset; return 0 or any value */
        return (seq_t)0;

    /* numerator and denominator of Φ₀ expressed as an exact rational:
       Φ₀ = 3313035075 / 1602176634 × 10^–15   (h = 6.62607015e‑34, e = 1.602176634e‑19) */
    static const idx_t NUM = 3313035075;
    static const idx_t DEN = 1602176634;

    idx_t pos = n + 14;   /* zero‑based index of the desired digit */

    if (pos == 0)          /* first digit of the integer part */
        return (seq_t)(NUM / DEN);

    /* produce fractional digits by long division */
    idx_t rem = NUM % DEN;
    seq_t digit = 0;
    for (idx_t i = 0; i < pos; ++i)
    {
        rem = rem * 10;
        digit = (seq_t)(rem / DEN);
        rem  = rem % DEN;
    }
    return digit;
}
