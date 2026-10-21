/*
 * A251420  Decimal expansion of Fisher's percolation exponent in two
 *          dimensions, 187/91.
 * https://oeis.org/A251420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1) = floor(187/91) = 2. For n > 1, a(n) is the nth digit after
 *            the decimal point of 187/91, which repeats the pattern
 *            0,5,4,9,4,5
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)(187 / 91);   /* integer part is 2 */

    /* cycle of decimal digits of 187/91 after the decimal point */
    static const seq_t cycle[6] = {0, 5, 4, 9, 4, 5};

    /* zero‑based position in the cycle:
       n == 2  -> 0
       n == 3  -> 1
       ...      */
    idx_t pos = (n - 2) % 6;
    return cycle[pos];
}
