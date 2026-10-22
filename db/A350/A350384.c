/*
 * A350384  a(n) = (-1728)^n.
 * https://oeis.org/A350384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1728)^n
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 2, i64 n <= 5, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = -(seq_t)1728;   /* negative base */
    idx_t exp    = n;              /* use a copy of n */

    while (exp > 0)
    {
        if (exp & 1U)              /* if lowest bit is set */
            result = result * base;
        base = base * base;        /* square the base */
        exp >>= 1;                 /* shift exponent right */
    }

    return result;
}
