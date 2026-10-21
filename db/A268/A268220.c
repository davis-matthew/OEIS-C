/*
 * A268220  a(n) = (n!/5!)*Sum(1/k!,k=1..n-5).
 * https://oeis.org/A268220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/5!)*Sum(1/k!,k=1..n-5)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6u) return (seq_t)0;
    seq_t result = (seq_t)0;
    for (idx_t i = 6u; i <= n; ++i) {
        seq_t prod = (seq_t)i;
        prod *= (seq_t)(i - 1u);
        prod *= (seq_t)(i - 2u);
        prod *= (seq_t)(i - 3u);
        prod *= (seq_t)(i - 4u);
        prod /= (seq_t)120u;
        result = (seq_t)i * result + prod;
    }
    return result;
}
