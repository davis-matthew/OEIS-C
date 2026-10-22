/*
 * A352177  a(n) is the number of symmetric Toeplitz anti-Hadamard matrices of
 *          order n whose sum of the inverse squares of their singular values
 *          is maximal.
 * https://oeis.org/A352177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 for n≥10; a(2)=2; a(4)=2; a(6)=3; a(9)=2; else a(n)=1
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)2;
    if (n == 4) return (seq_t)2;
    if (n == 6) return (seq_t)3;
    if (n == 9) return (seq_t)2;
    return (seq_t)1;
}
