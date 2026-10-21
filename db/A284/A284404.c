/*
 * A284404  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 913", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+1 for n>4; a(0)=1, a(1)=0, a(2)=101, a(3)=111,
 *            a(4)=11111
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)101;
    if (n == 3) return (seq_t)111;
    if (n == 4) return (seq_t)11111;

    seq_t res = (seq_t)11111;
    for (idx_t i = 5; i <= n; ++i)
        res = res * 10 + 1;
    return res;
}
