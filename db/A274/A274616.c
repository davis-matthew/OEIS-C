/*
 * A274616  Maximal number of non-attacking queens on a right triangular board
 *          with n cells on each side.
 * https://oeis.org/A274616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(2*n/3) with special case a(4)=2
 * Range:     u8 n <= 382, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 4)
        return 2;
    seq_t nn = (seq_t)n;
    return (nn * 2 + 1) / 3;
}
