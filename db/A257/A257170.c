/*
 * A257170  Expansion of (1 + x) * (1 + x^3) / (1 + x^4) in powers of x.
 * https://oeis.org/A257170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(0)=1; for n>0, a(n)=0 if n even, otherwise
 *            a(n)=(-1)^{floor(n/4)}
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if ((n & 1) == 0)        /* even n */
        return 0;
    idx_t k = n >> 2;        /* floor(n/4) */
    return (seq_t)(((k & 1) == 0) ? 1 : -1);
}
