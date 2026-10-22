/*
 * A328752  Lexicographically earliest sequence of distinct positive numbers
 *          such that for any n > 0, the first nonzero digit of a(n+1)/a(n) is
 *          "1".
 * https://oeis.org/A328752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for n > 11; otherwise the lexicographically earliest
 *            sequence of distinct positive integers such that for every n>0
 *            the first nonzero digit of a(n+1)/a(n) is 1. Explicit first
 *            terms: a(1)=1, a(2)=10, a(3)=11, a(4)=2, a(5)=3, a(6)=4, a(7)=5,
 *            a(8)=6, a(9)=7, a(10)=8, a(11)=9
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t specials[12] = {0, 1, 10, 11, 2, 3, 4, 5,
                                       6, 7, 8, 9};
    if (n <= 11)
        return specials[n];
    return (seq_t)n;
}
