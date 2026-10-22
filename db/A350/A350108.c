/*
 * A350108  a(n) = Sum_{k=1..n} k * floor(n/k)^3.
 * https://oeis.org/A350108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * floor(n/k)^3
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1379, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;                    // floor(n/k)
        seq_t sm = (seq_t)m;                // convert to seq_t
        seq_t m3 = sm * sm * sm;            // (n/k)^3
        res += (seq_t)k * m3;               // accumulate k*(n/k)^3
    }
    return res;
}
