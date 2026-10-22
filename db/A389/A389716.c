/*
 * A389716  a(n) is the maximum total surface area of three-element sets of
 *          distinct integer-sided cuboids that fill an n X n X n cube.
 * https://oeis.org/A389716
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*n^2 for n >= 6, * a(n) = 10*n^2 - 2*n for 3 <= n <= 5
 *            * (for n < 3 the value is undefined; return 0 here). * This
 *            formula reproduces the OEIS data for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    if (n <= 5) {
        return sq * 10 - nn * 2;
    } else {
        return sq * 10;
    }
}
