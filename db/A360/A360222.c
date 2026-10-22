/*
 * A360222  a(n) is the number of permutable pieces in a standard n X n X n
 *          Rubik's cube.
 * https://oeis.org/A360222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*n - 12*n + 8 - 6*(n mod 2) for n > 1; a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    seq_t sn = (seq_t)n;
    seq_t res = 6 * sn * sn - 12 * sn + 8 - 6 * (sn & 1);
    return res;
}
