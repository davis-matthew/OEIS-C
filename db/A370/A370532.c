/*
 * A370532  Integers m > 0 such that m^m and m^(m^m) have the same rightmost m
 *          digits.
 * https://oeis.org/A370532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1)=1, a(2)=5, a(n)=10*(n-2) for n>2
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    } else if (n == (idx_t)2) {
        return (seq_t)5;
    } else {
        return ((seq_t)10) * (seq_t)(n - (idx_t)2);
    }
}
