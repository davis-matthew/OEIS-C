/*
 * A301869  Decimal expansion of the nominal solar radius in meters.
 * https://oeis.org/A301869
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = digit of 695700000 at position (n−8), zeros thereafter
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t digits[9] = { 6, 9, 5, 7, 0, 0, 0, 0, 0 };
    if (n < 9)
        return 0;                   /* values below the offset are irrelevant */
    idx_t pos = n - 9;              /* zero‑based index into the digit array */
    if (pos < 9)
        return digits[pos];
    return 0;                      /* trailing zeros beyond the integer part */
}
