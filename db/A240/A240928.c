/*
 * A240928  Number of 8-digit positive integers in base n where the sum of the
 *          first k digits equals the sum of the last k digits.
 * https://oeis.org/A240928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n*(n-1)*(1208*n^5+151*n^4+291*n^3+116*n^2+88*n+60)/2520
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The definition is valid for n ≥ 2. For smaller n the value is unused. */
    if (n < 2) return 0;

    seq_t un = (seq_t)n;          /* convert index to seq_t */
    seq_t n1 = un - 1;
    seq_t n2 = un * un;
    seq_t n3 = n2 * un;
    seq_t n4 = n3 * un;
    seq_t n5 = n4 * un;

    /* compute the polynomial in the numerator */
    seq_t poly = 1208 * n5
               + 151 * n4
               + 291 * n3
               + 116 * n2
               + 88  * un
               + 60;

    /* full numerator and division by 2520 */
    seq_t numer = un * n1 * poly;
    return numer / 2520;
}
