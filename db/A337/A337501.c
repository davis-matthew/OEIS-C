/*
 * A337501  Minimum number of painted cells in an n X n grid to avoid
 *          unpainted trominoes.
 * https://oeis.org/A337501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2/2 if n even; a(n) = n*(n-1)/2 + floor(n/3) if n odd
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res;
    if ((n & 1u) == 0) /* even n */
    {
        res = ((seq_t)n * (seq_t)n) / 2;
    }
    else /* odd n */
    {
        res = ((seq_t)n * (seq_t)(n - 1)) / 2 + (seq_t)(n / 3);
    }
    return res;
}
