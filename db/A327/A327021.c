/*
 * A327021  a(n) = (2*n-1)! / 2^(n-1) if n > 0 and a(0) = 1.
 * https://oeis.org/A327021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)! / 2^(n-1) for n>0, a(0) = 1 Equivalent form used
 *            for efficient computation: a(n) = (n-1)! * Π_{k=1}^{n} (2k-1)
 *            (product of odd numbers up to 2n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Compute (n-1)!  */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n-1; ++i)
        fact *= (seq_t)i;

    /* Compute the product of odd numbers up to 2n-1: Π_{k=1}^{n} (2k-1) */
    seq_t odd = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        odd *= (seq_t)(2*k - 1);

    return fact * odd;
}
