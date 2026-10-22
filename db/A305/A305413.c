/*
 * A305413  a(n) = Fibonacci(11*n)/89.
 * https://oeis.org/A305413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 199*a(n-1) + a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0ULL) return (seq_t)0;
    if (n == 1ULL) return (seq_t)1;
    seq_t prev2 = (seq_t)0;      /* a(n-2) */
    seq_t prev1 = (seq_t)1;      /* a(n-1) */
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)199 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
