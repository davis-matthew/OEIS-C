/*
 * A392266  a(n) = (3^n + 2*n*3^(n-1) - 1)/4.
 * https://oeis.org/A392266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + 2*n*3^(n-1) - 1)/4
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n as a seq_t */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)3;

    /* compute 3^(n-1) by integer division */
    seq_t pow_prev = pow / (seq_t)3;

    /* temp = 2 * n * 3^(n-1) */
    seq_t temp = (seq_t)n * pow_prev;
    temp *= (seq_t)2;

    /* sum = 3^n + temp */
    seq_t sum = pow + temp;

    /* a(n) = (sum - 1) / 4 */
    return (sum - (seq_t)1) / (seq_t)4;
}
