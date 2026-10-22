/*
 * A369569  a(n) = (n-1)! * n^n.
 * https://oeis.org/A369569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * n^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is defined; for n < 1 just return 0 */
    if (n < 1)
        return (seq_t)0;

    /* Compute (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* Compute n^n */
    seq_t p = 1;
    for (idx_t i = 0; i < n; ++i)
        p *= (seq_t)n;

    return fact * p;
}
