/*
 * A375551  a(n) = Sum_{k=0..n} k XOR n-k, where XOR is the bitwise exclusive
 *          disjunction. Row sums of A003987.
 * https://oeis.org/A375551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k XOR (n-k)
 * Range:     u8 n <= 20, u16 n <= 316, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t x = (seq_t)k;
        seq_t y = (seq_t)(n - k);
        sum += x ^ y;
    }
    return sum;
}
