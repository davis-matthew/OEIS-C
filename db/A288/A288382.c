/*
 * A288382  Positions of 0 in A288381; complement of A288383.
 * https://oeis.org/A288382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) + n - 1 for n >= 3; a(1) = 1; a(2) = 2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t pow2 = (seq_t)1;
    pow2 <<= (n - 3);          /* 2^(n-3) */
    return pow2 + (seq_t)n - (seq_t)1;
}
