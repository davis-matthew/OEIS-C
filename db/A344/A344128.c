/*
 * A344128  a(n) = Sum_{k=1..n} k * floor(n/k^2).
 * https://oeis.org/A344128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * floor(n / k^2)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k * k <= n; ++k) {
        idx_t val = n / (k * k);
        res += (seq_t)k * (seq_t)val;
    }
    return res;
}
