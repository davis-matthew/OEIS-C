/*
 * A369597  a(n) is the number of reduced stable marriage problem instances of
 *          order 3 that generate n possible stable matchings.
 * https://oeis.org/A369597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n<1 or n>3; a(1) = 2840, a(2) = 957, a(3) = 91
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2840;
    if (n == 2) return (seq_t)957;
    if (n == 3) return (seq_t)91;
    return (seq_t)0;
}
