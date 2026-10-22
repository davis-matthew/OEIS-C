/*
 * A346689  Replace 5^k with (-1)^k in base-5 expansion of n.
 * https://oeis.org/A346689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Sum_{i=0}^{m} (-1)^i * d_i (d_i are the base‑5 digits of
 *            n, least significant digit first)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* running alternating digit sum */
    idx_t  t   = n;         /* working copy of n */
    int    sign = 1;        /* +1 for least significant digit */

    while (t > 0) {
        int digit = t % 5;          /* least significant base‑5 digit */
        sum += (seq_t)(sign * digit);  /* add or subtract it */
        sign = -sign;               /* alternate sign for next digit */
        t /= 5;                     /* shift to next digit */
    }

    return sum;
}
