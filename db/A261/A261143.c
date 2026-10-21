/*
 * A261143  a(n) = H_n(1,2) where H_n is the n-th hyperoperator.
 * https://oeis.org/A261143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=3, a(1)=3, a(2)=2, a(n)=1 for n>=3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)2;
    return (seq_t)1;
}
