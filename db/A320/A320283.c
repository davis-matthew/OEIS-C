/*
 * A320283  Lexicographical ordering of pure imaginary integers in the base
 *          (-1+i) numeral system.
 * https://oeis.org/A320283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(floor(n/4)) + a(n mod 4) for n ≥ 0; initial values:
 *            a(0)=0, a(1)=1, a(2)=-2, a(3)=-1
 * Range:     i8 n <= 127, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[4] = {0, 1, -2, -1};

    if (n < 4)             /* base cases */
        return base[n];

    seq_t q = a(n / 4);    /* a(floor(n/4)) */
    seq_t r = a(n % 4);    /* a(n mod 4)   */

    q *= 4;                /* multiply by 4 */
    q = -q;                /* negate */
    return q + r;          /* result: -4*a(floor(n/4)) + a(n mod 4) */
}
