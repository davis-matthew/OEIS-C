/*
 * A382394  a(n) = Sum_{k=0..n} A128899(n,k)^3.
 * https://oeis.org/A382394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003161(2*n-1) = Sum_{k=0..n-1} (binomial(2n-1,k) -
 *            binomial(2n-1,k-1))^3 for n > 0; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t m = 2 * n - 1;
    idx_t half = m / 2;
    seq_t result = 1;   /* k = 0 term */
    seq_t prev = 1;     /* binomial(m, k-1) */

    for (idx_t k = 1; k <= half; k++) {
        seq_t cur = prev * (seq_t)(m - k + 1) / (seq_t)k;
        seq_t diff = cur - prev;
        result += diff * diff * diff;
        prev = cur;
    }
    return result;
}
