/*
 * A344711  a(n) = Sum_{k=1..n} (A000330(n) mod k^2).
 * https://oeis.org/A344711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (A000330(n) mod k^2)
 * Range:     u8 n <= 8, u16 n <= 69, u32 n <= 1673, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000330(n) = 1^2 + 2^2 + ... + n^2 = n(n+1)(2n+1)/6 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* sequence is defined for n >= 1 */
    seq_t N = (seq_t)n;
    seq_t s = N * (N + 1) * (2 * N + 1) / 6;  /* sum of squares */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k2 = (seq_t)k * (seq_t)k;   /* k^2 */
        sum += s % k2;
    }
    return sum;
}
