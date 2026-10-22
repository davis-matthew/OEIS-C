/*
 * A338379  Floor of (digital root of n) divided by (number of digits in n).
 * https://oeis.org/A338379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(digital_root(n) / digits_in_decimal(n))
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digital root: 0 for n==0, otherwise 1..9 via modulo 9 */
    seq_t dr;
    if (n == 0)
        dr = 0;
    else
    {
        dr = (seq_t)(n % 9);
        if (dr == 0)
            dr = 9;
    }

    /* count number of decimal digits */
    idx_t digits = 1;
    idx_t t = n;
    while (t >= 10)
    {
        t /= 10;
        ++digits;
    }

    return dr / (seq_t)digits;
}
