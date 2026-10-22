/*
 * A377124  Phase shift (original name "sfasamento") of the tetration base
 *          10*n at any height greater than or equal to 3.
 * https://oeis.org/A377124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if the least significant non‑zero digit of n is in
 *            {1,3,7,9}, * 5 if it is 5, * 6 otherwise (digits 2,4,6,8)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the least significant non‑zero decimal digit of n. */
    idx_t m = n;
    if (m == 0)               /* n should never be 0 because offset is 1 */
        return (seq_t)1;

    while (m % 10 == 0)
        m /= 10;               /* strip trailing zeros */
    uint8_t h = m % 10;        /* last non‑zero digit */

    /* Map last non‑zero digit to the phase shift value. */
    switch (h) {
        case 1: case 3: case 7: case 9:
            return (seq_t)1;   /* a(n) = 1 */
        case 5:
            return (seq_t)5;   /* a(n) = 5 */
        default:               /* digits 2,4,6,8 */
            return (seq_t)6;   /* a(n) = 6 */
    }
}
