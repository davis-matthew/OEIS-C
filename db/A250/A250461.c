/*
 * A250461  Number of (n+1)X(1+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-3), for n ≥ 4 with initial values: a(1)
 *            = 11, a(2) = 31, a(3) = 85
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)31;
    if (n == 3) return (seq_t)85;

    /* iterative computation using only the last three values */
    seq_t a1 = (seq_t)11; /* a(n-3) */
    seq_t a2 = (seq_t)31; /* a(n-2) */
    seq_t a3 = (seq_t)85; /* a(n-1) */
    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 2*a(i-3) */
        next = 3 * a3 - 2 * a1;
        /* shift the window: a1 <- a2, a2 <- a3, a3 <- next */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
