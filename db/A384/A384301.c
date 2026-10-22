/*
 * A384301  a(n) = Product_{k=0..2*n-1} (3*n+k-1).
 * https://oeis.org/A384301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0}^{2*n-1} (3*n + k - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* Loop over the 2*n terms of the rising factorial (3*n-1)_{2*n} */
    for (idx_t i = 0; i < 2 * n; ++i)
    {
        /* Term: 3*n - 1 + i  (all computations done in seq_t) */
        seq_t term = (seq_t)(3 * n - 1 + i);
        result *= term;
    }
    return result;
}
