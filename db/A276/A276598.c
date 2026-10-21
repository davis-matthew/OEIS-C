/*
 * A276598  Values of m such that m^2 + 3 is a triangular number (A000217).
 * https://oeis.org/A276598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * a(n-1) - a(n-2) with a(1)=0, a(2)=5 (A276598
 *            recurrence)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;

    seq_t prev2 = (seq_t)0;  /* a(1) */
    seq_t prev1 = (seq_t)5;  /* a(2) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur   = 6 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
