/*
 * A291001  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 8*S^2.
 * https://oeis.org/A291001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2); Initial values: a(0) = 0, a(1) = 8,
 *            a(2) = 16
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 8;
    if (n == 2) return 16;

    seq_t prev2 = 8;   /* a(1) */
    seq_t prev1 = 16;  /* a(2) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur   = 2 * prev1 + 7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
