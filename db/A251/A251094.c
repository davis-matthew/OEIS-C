/*
 * A251094  Number of (n+1) X (2+1) 0..1 arrays with no 2 X 2 subblock having
 *          its minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A251094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) + 10*a(n-3) - 3*a(n-4); Initial
 *            terms: a(1)=41, a(2)=212, a(3)=1109, a(4)=5817
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1 */
        return 0;
    if (n == 1) return 41;
    if (n == 2) return 212;
    if (n == 3) return 1109;
    if (n == 4) return 5817;

    seq_t prev4 = 41;   /* a(1) */
    seq_t prev3 = 212;  /* a(2) */
    seq_t prev2 = 1109; /* a(3) */
    seq_t prev1 = 5817; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = 7*prev1 - 11*prev2 + 10*prev3 - 3*prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
