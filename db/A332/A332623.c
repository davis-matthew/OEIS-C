/*
 * A332623  a(n) = Sum_{k=1..n} ceiling(n/k)^2.
 * https://oeis.org/A332623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} ceil(n/k)^2
 * Range:     u8 n <= 12, u16 n <= 198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* ceil(n/k) = (n + k - 1) / k */
        idx_t q = (n + k - 1) / k;
        seq_t t = (seq_t)q;
        sum += t * t;
    }
    return sum;
}
