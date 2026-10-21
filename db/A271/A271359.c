/*
 * A271359  a(n) = k*Fibonacci(2*n+1) + (k+1)*Fibonacci(2*n), where k=5.
 * https://oeis.org/A271359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) a(0) = 5, a(1) = 16
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)16;

    seq_t prev2 = (seq_t)5;
    seq_t prev1 = (seq_t)16;
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
