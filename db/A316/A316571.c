/*
 * A316571  a(1) = 1; for n > 1: a(n) = smallest number such that
 *          (Sum_{k=1..n} a(k)) is divisible by n - 1.
 * https://oeis.org/A316571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=2; a(3)=3; a(n)=2*(n-1) for n>=4
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    return (seq_t)2 * ((seq_t)n - 1);
}
