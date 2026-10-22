/*
 * A347513  Number of minimal dominating sets in the n-cycle complement graph.
 * https://oeis.org/A347513
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-3)/2 for n > 6; special cases: a(3)=1, a(4)=4,
 *            a(5)=5, a(6)=11
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    if (n == 3)
        return 1;
    if (n == 4)
        return 4;
    if (n == 5)
        return 5;
    if (n == 6)
        return 11;
    seq_t nm = (seq_t)n;
    seq_t temp = nm - 3;
    return nm * temp / 2;
}
