/*
 * A355258  a(n) = n! * [x^n] (1 - x)*log((1 - x)/(1 - 2*x)).
 * https://oeis.org/A355258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + 2^(n-1) * (n-2)) * (n-2)! for n >= 2, a(0) = 0, a(1)
 *            = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* compute (n-2)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 2; ++i)
        fact *= (seq_t)i;

    /* compute 2^(n-1) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i <= n - 1; ++i)
        pow2 *= (seq_t)2;

    /* (1 + (n-2) * 2^(n-1)) * (n-2)! */
    seq_t term = (seq_t)1 + pow2 * (seq_t)(n - 2);
    return term * fact;
}
