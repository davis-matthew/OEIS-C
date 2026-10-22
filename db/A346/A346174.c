/*
 * A346174  Inverse binomial transform of A317614.
 * https://oeis.org/A346174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4)*n*(n+1)*(n+2) for n >= 4, * a(0) = 0, a(1) = 1,
 *            a(2) = 6, a(3) = 30
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)30;

    /* Compute 2^(n-4) by repeated multiplication to support arbitrary precision */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n - 4; ++i)
        pow2 *= (seq_t)2;

    /* Multiply by n*(n+1)*(n+2) */
    seq_t result = pow2;
    result *= (seq_t)n;
    result *= (seq_t)(n + 1);
    result *= (seq_t)(n + 2);

    return result;
}
