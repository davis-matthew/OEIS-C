/*
 * A353906  a(n) is the {alternating sum of the digits of n} raised to the
 *          power {number of digits of n}.
 * https://oeis.org/A353906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   a(n) = (alternating sum of digits of n)^(number of digits of n)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;          /* working copy of n */
    int len = 0;          /* number of digits */
    seq_t alt = 0;        /* alternating sum of digits */
    int sign = 1;         /* +1 or -1 for alternating signs */

    /* Compute the alternating sum and the number of digits */
    while (m > 0) {
        int digit = m % 10;          /* current least‑significant digit */
        alt += (seq_t)sign * digit;  /* add/subtract digit */
        sign = -sign;                /* flip sign for the next digit */
        m /= 10;                     /* remove processed digit */
        ++len;
    }

    /* Compute alt^len (len >= 1 because n >= 1) */
    seq_t result = 1;
    for (int i = 0; i < len; ++i)
        result *= alt;

    return result;
}
