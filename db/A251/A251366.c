/*
 * A251366  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 1 2 3 4 5 6 or 7.
 * https://oeis.org/A251366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 7*a(n-2); a(1)=79, a(2)=695
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 79;
    if (n == 2) return 695;
    seq_t a0 = 79;
    seq_t a1 = 695;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = a1 * 8 + a0 * 7;
        a0 = a1;
        a1 = cur;
    }
    return a1;
}
