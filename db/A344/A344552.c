/*
 * A344552  a(n) = Sum_{k=1..n} floor(k*(n-k)/n).
 * https://oeis.org/A344552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(k*(n-k)/n)
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t prod = (seq_t)k * (seq_t)(n - k);  /* k*(n-k) as seq_t */
        sum += prod / (seq_t)n;                  /* floor division */
    }
    return sum;
}
