/*
 * A337929  Numbers w such that (F(2*n-1)^2, -F(2*n)^2, w) are primitive
 *          solutions of the Diophantine equation 2*x^3 + 2*y^3 + z^3 = 1,
 *          where F(n) is the n-th Fibonacci number (A000045).
 * https://oeis.org/A337929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) with a(1)=1, a(2)=11,
 *            a(3)=79
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 11;
    if (n == 3) return 79;

    seq_t a1 = 1;
    seq_t a2 = 11;
    seq_t a3 = 79;
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        curr = a3 * 8 - a2 * 8 + a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return a3;
}
