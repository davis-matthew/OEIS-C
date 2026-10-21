/*
 * A255820  Decimal expansion of the heliocentric gravitational constant in SI
 *          units.
 * https://oeis.org/A255820
 *
 * Offset:    21    (a(21) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = digit at position n of the decimal expansion of 1.3271244*10^20
   (the heliocentric gravitational constant); the sequence has offset 21,
   so a(21) is the first listed digit.  For n≥21, the value is the digit
   at index (n−21).  All further digits are 0. */

seq_t a(idx_t n)
{
    /* the first eight digits of 1.3271244*10^20 = 13271244*10^13 */
    static const seq_t digits[8] = { 1, 3, 2, 7, 1, 2, 4, 4 };

    if (n < 21) {
        /* values below the offset are unspecified; return 0 */
        return (seq_t)0;
    }

    idx_t pos = n - (idx_t)21;
    if (pos < (idx_t)8) {
        return digits[pos];
    }
    /* all subsequent digits are 0 */
    return (seq_t)0;
}
