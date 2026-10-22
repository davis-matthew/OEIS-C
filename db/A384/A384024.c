/*
 * A384024  a(n) = [x^n] Product_{k=0..n} (1 + (n+k)*x).
 * https://oeis.org/A384024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=0..n} (1 + (n+k)x) Equivalent
 *            combinatorial expression: a(n) = Σ_{j=0}^n ∏_{k≠j} (n+k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute the product P = Π_{t=n}^{2n} t = (2n)!/(n-1)!   */
    seq_t prod = 1;
    for (idx_t t = n; t <= 2 * n; ++t)
        prod *= (seq_t)t;

    /* sum all terms prod/(n+j), j = 0..n   */
    seq_t result = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = prod / (seq_t)(n + j);
        result += term;
    }
    return result;
}
