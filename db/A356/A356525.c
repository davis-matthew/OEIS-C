/*
 * A356525  Decimal expansion of number of Pascals (Pa) in 1 millimeter of
 *          mercury (mmHg).
 * https://oeis.org/A356525
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   decimal digits of 133.322387415 Pa, offset 3
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const char digits[] = "133322387415";

seq_t a(idx_t n)
{
    if (n < 3u)
        return (seq_t)0;   /* undefined for n < 3 */

    idx_t pos = n - 3u;
    if (pos < sizeof(digits) - 1u)   /* 12 digits */
        return (seq_t)(digits[pos] - '0');
    return (seq_t)0;                 /* trailing zeros after the constant */
}
