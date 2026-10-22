/*
 * A390140  Centered truncated cube numbers: a(n) = (46*n^3 - 69*n^2 + 29*n -
 *          3)/3.
 * https://oeis.org/A390140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (46*n^3 - 69*n^2 + 29*n - 3)/3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 49, a(3) = 235, ... */

seq_t a(idx_t n)
{
    /* For offset 1, n should be >= 1; for n == 0 a trivial value is returned. */
    if (n == 0) return 0;

    seq_t N = (seq_t)n;
    seq_t three = (seq_t)3;

    /* Compute numerator safely in the seq_t type. */
    seq_t numerator = 46 * N * N * N
                    - 69 * N * N
                    + 29 * N
                    - (seq_t)3;

    return numerator / three;
}
