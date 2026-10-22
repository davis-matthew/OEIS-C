/*
 * A352601  a(n) = RisingFactorial(2*n, n) = A124320(2*n, n).
 * https://oeis.org/A352601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)(2n+1)...(3n-1) (rising factorial of 2n, length n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition of empty product */
    if (n == 0) {
        return 1;
    }

    /* start = 2 * n  (converted to seq_t) */
    seq_t start = (seq_t)n * (seq_t)2;
    seq_t result = 1;

    /* multiply the terms 2n, 2n+1, ..., 2n+n-1 */
    for (idx_t k = 0; k < n; ++k) {
        result *= (start + (seq_t)k);
    }

    return result;
}
