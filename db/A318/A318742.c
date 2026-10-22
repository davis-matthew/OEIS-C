/*
 * A318742  a(n) = Sum_{k=1..n} floor(n/k)^3.
 * https://oeis.org/A318742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)^3
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 1529, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;                   /* floor(n/k) */
        seq_t t = (seq_t)q * (seq_t)q * (seq_t)q; /* q^3 */
        sum += t;
    }
    return sum;
}
