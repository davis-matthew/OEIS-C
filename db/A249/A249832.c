/*
 * A249832  Characteristic function of numbers having a valid Luhn mod 10
 *          check digit.
 * https://oeis.org/A249832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000007(A093017(n) mod 10) i.e. a(n) = 1 iff the decimal
 *            representation of n satisfies the Luhn mod‑10 check digit;
 *            otherwise a(n) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t sum = 0;          /* running Luhn checksum sum */
    int double_flag = 0;    /* 0 = don't double; 1 = double this digit */

    /* Process all decimal digits, including the special case n == 0 */
    do
    {
        idx_t d = n % 10;   /* current least‑significant digit */

        if (double_flag)
        {
            /* Double the digit and subtract 9 if the result exceeds 9 */
            d = d * 2;
            if (d > 9) d -= 9;
        }

        sum += d;          /* accumulate checksum */
        double_flag = 1 - double_flag; /* toggle doubling for next digit */
        n /= 10;           /* move to next higher digit */
    } while (n > 0);

    /* Return 1 (true) if checksum is a multiple of 10, else 0 */
    return (seq_t)((sum % 10) == 0);
}
