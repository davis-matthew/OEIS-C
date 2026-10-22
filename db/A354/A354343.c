/*
 * A354343  Number of distinct sums of n complex 6th power roots of unity.
 * https://oeis.org/A354343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 3*n + 1 for n >= 2; a(0) = 1, a(1) = 6
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t nn = (seq_t)n;
    seq_t res = 3 * nn * nn;
    res += 3 * nn;
    res += 1;
    return res;
}
