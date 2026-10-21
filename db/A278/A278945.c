/*
 * A278945  Expansion of Sum_{k>=1} k*(2*k - 1)*x^k/(1 - x^k).
 * https://oeis.org/A278945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d*(2*d - 1)
 * Range:     u8 n <= 11, u16 n <= 155, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t d_s = (seq_t)d;
            sum += d_s * ((seq_t)2 * d_s - 1);

            idx_t other = n / d;
            if (other != d) {
                seq_t o_s = (seq_t)other;
                sum += o_s * ((seq_t)2 * o_s - 1);
            }
        }
    }
    return sum;
}
