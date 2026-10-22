/*
 * A316650  Result when n is divided by the sum of its digits and the
 *          resulting integer is concatenated to the remainder.
 * https://oeis.org/A316650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = concatenate( floor(n / digit_sum(n)), n mod digit_sum(n)
 *            )
 * Range:     u8 n <= 48, u16 n <= 9111, u32 n <= 9673, u64 n <= 9673,
 *            u128 n <= 9673, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n = 0, return 0 */

    /* compute the sum of decimal digits of n */
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp) {
        sum += tmp % 10;
        tmp /= 10;
    }

    seq_t q = (seq_t)n / sum;   /* integer quotient */
    seq_t r = (seq_t)n % sum;   /* remainder */

    /* determine the number of decimal digits of r (count 0 as 1 digit) */
    int digits = 1;
    seq_t rem = r;
    if (rem != 0) {
        digits = 0;
        while (rem > 0) {
            digits++;
            rem /= 10;
        }
    }

    /* compute 10^digits */
    seq_t pow10 = 1;
    for (int i = 0; i < digits; ++i)
        pow10 *= 10;

    /* concatenate q and r: q * 10^digits + r */
    return q * pow10 + r;
}
