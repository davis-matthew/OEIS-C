/*
 * A296063  a(n) is the smallest number (in absolute value) not yet in the
 *          sequence such that the arithmetic mean of the first n terms a(1),
 *          a(2), ..., a(n) is an integer. Preference is given to positive
 *          values of a(n); a(1)=1; 0 not allowed.
 * https://oeis.org/A296063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1)+a(n-2)+a(n-3), a(1)=1, a(2)=-1, a(3)=3.
 *            Equivalent explicit form: a(n) = n for odd n, a(n) = -(n-1) for
 *            even n
 * Range:     i8 n <= 128, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    if ((n & 1ULL) != 0ULL)          /* odd index */
    {
        return (seq_t)n;
    }
    else                            /* even index */
    {
        seq_t m = (seq_t)(n - 1);
        return -m;
    }
}
