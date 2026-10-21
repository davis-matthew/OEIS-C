/*
 * A289741  a(n) = Kronecker symbol (-20/n).
 * https://oeis.org/A289741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Kronecker symbol (-20/n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n coprime with 20 the value is (-1)^⌊n/10⌋, otherwise 0. */
seq_t a(idx_t n)
{
    /* Return 0 if n shares a factor with 20 (i.e., 2 or 5). */
    if ((n % 2) == 0 || (n % 5) == 0)
        return (seq_t)0;

    /* Compute floor(n/10). Use idx_t division, then test parity. */
    idx_t t = n / 10;
    return (t & 1) ? (seq_t)-1 : (seq_t)1;
}
