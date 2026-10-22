/*
 * A329509  Expansion of (1 + x)*(1 + x + x^2 - x^3) / (1 - x).
 * https://oeis.org/A329509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2x + 2x² - x⁴) / (1 - x) => a(0)=1, a(1)=3, a(2)=5,
 *            a(3)=5, a(n)=4 for n≥4
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 5;
    if (n == 3) return 5;
    return 4;
}
