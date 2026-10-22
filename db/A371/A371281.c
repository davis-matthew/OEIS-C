/*
 * A371281  Last digit of the product of decimal digits of n.
 * https://oeis.org/A371281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007954(n) mod 10 * Last digit of the product of the
 *            decimal digits of n. * a(0) = 0 (by convention: single 0 digit)
 *            * a(n) = (∏ decimal digits of n) mod 10 for n > 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((idx_t)0 == n) return (seq_t)0;

    unsigned int prod = (unsigned int)1;
    idx_t m = n;
    while (m > (idx_t)0) {
        unsigned int d = (unsigned int)(m % (idx_t)10);
        if (d == (unsigned int)0) return (seq_t)0;   /* product becomes 0 */
        prod = (prod * d) % (unsigned int)10;
        m = m / (idx_t)10;
    }
    return (seq_t)prod;
}
