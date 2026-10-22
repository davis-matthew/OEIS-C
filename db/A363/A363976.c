/*
 * A363976  Expansion of Sum_{k>0} x^(2*k) / (1 - x^(3*k))^3.
 * https://oeis.org/A363976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d ≡ 2 (mod 3)} binomial((d+1)/3 + 1, 2)
 * Range:     u8 n <= 64, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 % 3 == 2) {
                idx_t m = (d1 + 1) / 3;
                sum += ((seq_t)(m + 1) * (seq_t)m) / 2;
            }
            idx_t d2 = n / i;
            if (d2 != d1 && d2 % 3 == 2) {
                idx_t m = (d2 + 1) / 3;
                sum += ((seq_t)(m + 1) * (seq_t)m) / 2;
            }
        }
    }
    return sum;
}
