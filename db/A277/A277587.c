/*
 * A277587  Decimal expansion of the sum of the reciprocals of the Wieferich
 *          primes.
 * https://oeis.org/A277587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, hard, more
 * Formula:   a(n) = nth decimal digit of (1/1093 + 1/3511) (1/1093 + 1/3511 =
 *            4604/3837523)
 * Range:     u8 none, u16 none, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Numerator and denominator of the exact rational sum of reciprocals of the known Wieferich primes. */
    const seq_t num = 4604;
    const seq_t den = 3837523;

    seq_t r = num;          /* current remainder in the long division */
    seq_t digit = 0;        /* digit produced in the current step */

    /* Compute digits one by one until the nth digit is obtained. */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10;            /* bring down next digit */
        digit = r / den;    /* integer part of division = next decimal digit */
        r %= den;           /* remainder for next step */
    }
    return digit;          /* return the nth digit (0‑based index) */
}
