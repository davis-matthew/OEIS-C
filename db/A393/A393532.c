/*
 * A393532  The difference between the maximum and minimum voucher costs for n
 *          distinct positive integer vouchers v_1, v_2, ..., v_n, where the
 *          cost is defined as v_1 + v_1*v_2 + v_2*v_3 + ... + v_(n-1)*v_n.
 * https://oeis.org/A393532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+3*n-19)/6 + (51-3*(-1)^n)/12, for n > 1; a(1)=0
 * Range:     u8 n <= 11, u16 n <= 72, u32 n <= 2952, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    long long nn = (long long)n;
    long long inner = nn * nn + 3 * nn - 19;
    long long epsilon = (nn & 1) ? -1 : 1;          /* (-1)^n */
    long long numerator = 2 * nn * inner + 51 - 3 * epsilon;
    return (seq_t)(numerator / 12);
}
