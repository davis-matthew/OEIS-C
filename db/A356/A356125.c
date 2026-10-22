/*
 * A356125  a(n) = Sum_{k=1..n} k * sigma_2(k).
 * https://oeis.org/A356125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * sigma_2(k)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 345, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent direct calculation: a(n) = Σ_{d=1..n} d^3 * floor(n/d) * (floor(n/d)+1) / 2 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t t = n / d;                 /* floor(n/d) */
        seq_t d3 = (seq_t)d * (seq_t)d * (seq_t)d;
        seq_t pairs = (seq_t)t * (t + 1) / 2;
        sum += d3 * pairs;
    }
    return sum;
}
