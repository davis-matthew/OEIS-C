/*
 * A356381  (Negated) Decimal expansion of value of absolute zero in degrees
 *          Celsius.
 * https://oeis.org/A356381
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = the (n-3)-rd digit of 27315 (digits indexed from 0), and
 *            a(n) = 0 for n ≥ 8. The decimal expansion of absolute zero in °C
 *            is 273.15
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digits of 27315 */
    static const seq_t digits[5] = {2, 7, 3, 1, 5};
    idx_t idx = n - 3;           /* offset adjustment */
    if (idx < 5)
        return digits[idx];
    else
        return 0;
}
