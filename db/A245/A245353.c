/*
 * A245353  Sum of digits of n written in fractional base 9/7.
 * https://oeis.org/A245353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of 'n' written in fractional base 9/7
 * Range:     u8 n <= 2183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t p = (seq_t)9;   /* base numerator */
    const seq_t q = (seq_t)7;   /* base denominator */
    seq_t val = (seq_t)n;
    seq_t sum = 0;

    while (val > 0)
    {
        seq_t d   = val % p;               /* least significant digit */
        sum += d;                          /* accumulate digit sum */
        val   = ((val - d) / p) * q;       /* shift to next higher position */
    }
    return sum;
}
