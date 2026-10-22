/*
 * A382019  Number of zeros (counted with multiplicity) inside and on the unit
 *          circle of the polynomial P(n,z) = Sum_{k=0..n} T(n,k)*z^k where
 *          T(n,k) = A214292(n,k) is the first differences of rows in Pascal's
 *          triangle.
 * https://oeis.org/A382019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0, a(n) = n - 2*floor((n-1)/6) for n >= 1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* floor((n-1)/6) can be computed with unsigned division */
    idx_t q = (n - 1) / 6;           /* quotient = floor((n-1)/6) */
    seq_t diff = (seq_t)2 * (seq_t)q; /* 2*floor((n-1)/6) */
    return (seq_t)n - diff;
}
