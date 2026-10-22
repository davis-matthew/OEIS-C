/*
 * A384300  a(n) = Product_{k=0..2*n-1} (3*n+k-2).
 * https://oeis.org/A384300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..2*n-1} (3*n + k - 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The product is empty for n == 0, yielding 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t result = 1;
    /* Compute each term of the rising factorial: (3*n - 2), (3*n - 1), ...,
       up to (3*n + 2*n - 3).  We use seq_t arithmetic to avoid overflow
       for the big‑integer build. */
    for (idx_t i = 0; i < 2 * n; ++i) {
        seq_t term = (seq_t)n * (seq_t)3 + (seq_t)i - (seq_t)2;
        result *= term;
    }
    return result;
}
