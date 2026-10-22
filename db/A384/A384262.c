/*
 * A384262  a(n) = Product_{k=0..n-1} (3*n+k-2).
 * https://oeis.org/A384262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (3*n + k - 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Product of consecutive integers from (3*n-2) to (4*n-3) */
seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 0; k < n; ++k)
    {
        /* each factor fits in seq_t when the final product does */
        res *= (seq_t)(3 * n + k - 2);
    }
    return res;
}
