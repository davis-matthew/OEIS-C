/*
 * A369568  Minimum number of edges that force an n-vertex graph to be
 *          Hamiltonian according to Chvátal's condition on the degree
 *          sequence.
 * https://oeis.org/A369568
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n^2+6*n)/16) if n even, *
 *            floor((3*n^2+8*n-3)/16) if n odd
 * Range:     u8 n <= 8, u16 n <= 409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;      /* values below offset are undefined */
    seq_t sn   = (seq_t)n;
    seq_t sum;
    if ((sn & 1) == 0)          /* n is even */
        sum = 3*sn*sn + 6*sn;
    else                        /* n is odd */
        sum = 3*sn*sn + 8*sn - 3;
    return sum / 16;
}
