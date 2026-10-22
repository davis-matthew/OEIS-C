/*
 * A337899  Number of chiral pairs of colorings of the edges of a regular
 *          tetrahedron using n or fewer colors.
 * https://oeis.org/A337899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * n^2 * (n+1) * (n^2-2) / 24
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 21, ... (OEIS A337899) */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly to avoid underflow in the
       (n^2 - 2) term when n == 1. */
    if (n <= 1)
        return (seq_t)0;

    /* Compute the polynomial in a order that keeps intermediate
       values as small as possible.  All arithmetic is performed in
       seq_t after casting idx_t operands to seq_t. */
    seq_t n_sq = (seq_t)n * (seq_t)n;           /* n^2 */
    seq_t prod = (seq_t)(n - 1);                 /* n-1 */
    prod *= n_sq;                               /* (n-1)*n^2 */
    prod *= (seq_t)(n + 1);                     /* * (n+1) */
    prod *= (n_sq - (seq_t)2);                  /* * (n^2-2) */
    return prod / (seq_t)24;                    /* divide by 24 */
}
