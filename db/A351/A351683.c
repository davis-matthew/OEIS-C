/*
 * A351683  Squares that are also 4-dimensional pyramidal numbers.
 * https://oeis.org/A351683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007655(n)^2 where A007655 satisfies * A(1)=0, A(2)=1,
 *            and for n>2: A(n) = 14*A(n-1) - A(n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A351683 sequence term for 1-based index n.  */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */
    if (n == 2) return 1;          /* a(2) = 1 */

    seq_t prevprev = 0;            /* A(1) */
    seq_t prev     = 1;            /* A(2) */
    seq_t current  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        current = prev * 14 - prevprev;  /* A(i) = 14*A(i-1) - A(i-2) */
        prevprev = prev;
        prev     = current;
    }

    /* Return square of A(n) */
    return prev * prev;
}
