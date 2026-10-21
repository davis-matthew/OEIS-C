/*
 * A261694  a(n) = Fibonacci(n) mod 21.
 * https://oeis.org/A261694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 21
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 21;
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t a0 = 0;
    seq_t a1 = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = (a0 + a1) % MOD;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
