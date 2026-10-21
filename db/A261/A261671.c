/*
 * A261671  If n even, a(n) = 6n+3, otherwise a(n) = n.
 * https://oeis.org/A261671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 3*(-1)^n + 7*n + 5*(-1)^n*n) / 2
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent piecewise form: if n is even a(n)=6*n+3 else a(n)=n */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)      /* even n */
        return (seq_t)(6 * n + 3);
    else                   /* odd n */
        return (seq_t)n;
}
