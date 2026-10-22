/*
 * A363975  Expansion of Sum_{k>0} x^k / (1 - x^(3*k))^3.
 * https://oeis.org/A363975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d ≡ 1 (mod 3)} binomial((d+5)/3, 2)
 * Range:     u8 n <= 63, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor i */
            if (i % 3 == 1) {
                seq_t d = (seq_t)i;
                seq_t m = ((d + 5) / 3);
                sum += m * (m - 1) / 2;
            }
            /* paired divisor n/i */
            idx_t j = n / i;
            if (j != i && j % 3 == 1) {
                seq_t d = (seq_t)j;
                seq_t m = ((d + 5) / 3);
                sum += m * (m - 1) / 2;
            }
        }
    }
    return sum;
}
