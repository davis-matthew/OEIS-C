/*
 * A329771  G.f. = (3*x^8 + x^7 - 3*x^6 - 3*x^5 - 3*x^4 - 3*x^3 - 3*x^2 - 2*x
 *          - 1)/(x - 1).
 * https://oeis.org/A329771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 3*n for 1 ≤ n ≤ 6; a(7) = 17; a(n) = 14 for n ≥
 *            8
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n <= 6)
        return (seq_t)(3 * n);

    if (n == 7)
        return (seq_t)17;

    /* n >= 8 */
    return (seq_t)14;
}
