/*
 * A318743  a(n) = Sum_{k=1..n} floor(n/k)^4.
 * https://oeis.org/A318743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;
        seq_t t = (seq_t)m;
        sum += t * t * t * t;
    }
    return sum;
}
