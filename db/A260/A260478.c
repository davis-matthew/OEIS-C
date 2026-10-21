/*
 * A260478  Cyclotomic polynomial value Phi(8,n!).
 * https://oeis.org/A260478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^4 + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n! iteratively in seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= i;
    }

    /* Compute (n!)^4 + 1 */
    seq_t sq  = fact * fact; /* (n!)^2 */
    seq_t res = sq * sq;     /* (n!)^4 */
    res += 1;
    return res;
}
