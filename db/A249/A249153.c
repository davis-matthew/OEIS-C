/*
 * A249153  Exponent of 2 in the hyperfactorial of 2n: a(n) =
 *          A007814(A002109(2n)).
 * https://oeis.org/A249153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..2n} k * v2(k), where v2(k) is the exponent of 2
 *            in the prime factorisation of k
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t two_n = 2 * n;
    for (idx_t k = 1; k <= two_n; ++k) {
        idx_t t = k;
        idx_t v = 0;
        while ((t & 1) == 0) {
            ++v;
            t >>= 1;
        }
        total += (seq_t)k * (seq_t)v;
    }
    return total;
}
