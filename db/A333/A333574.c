/*
 * A333574  Number of Hamiltonian paths in the n X 2 grid graph which start at
 *          any of the n vertices on left side of the graph and terminate at
 *          any of the n vertices on the right side.
 * https://oeis.org/A333574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 - 4*n + 9 - (-1)^n)/4 for n > 1, a(1) = 1
 * Range:     u8 n <= 12, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Compute numerator: 2*n^2 - 4*n + 9 - (-1)^n
       Using unsigned arithmetic: add 8 for even n, 10 for odd n,
       because 9 - (-1)^n equals 8 when n is even, and 10 when n is odd. */
    seq_t n2 = (seq_t)n;
    seq_t base = (seq_t)2 * n2 * n2 - (seq_t)4 * n2;
    if ((n & 1) == 0)   /* n is even */
        base += (seq_t)8;
    else                /* n is odd */
        base += (seq_t)10;

    return base / 4;
}
