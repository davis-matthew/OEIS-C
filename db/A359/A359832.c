/*
 * A359832  a(n) = 1 if the 2-adic valuation of n is either 0 or odd,
 *          otherwise 0.
 * https://oeis.org/A359832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if the 2-adic valuation of n is 0 or odd, otherwise 0
 * Range:     u8 n <= 9660, u16 n <= 9660, u32 n <= 9660, u64 n <= 9660,
 *            u128 n <= 9660, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    idx_t e = 0;                 /* exponent of 2 in n */
    idx_t t = n;

    /* Count trailing zeros of n (the 2-adic valuation) */
    while ((t & 1) == 0) {
        t >>= 1;
        e++;
    }

    /* Return 1 if e == 0 (n odd) or e is odd (e > 0 and odd), otherwise 0 */
    if (e == 0 || (e & 1) != 0)
        return (seq_t)1;
    else
        return (seq_t)0;
}
