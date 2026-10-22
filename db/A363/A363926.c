/*
 * A363926  Expansion of Sum_{k>0} x^(2*k) / (1 - x^(5*k))^2.
 * https://oeis.org/A363926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d|n, d==2 mod 5} (d+3)
 * Range:     u8 n <= 1091, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* iterate divisors i up to sqrt(n) using n/i to avoid overflow   */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 % 5 == 2) {
                sum += (seq_t)d1 + 3;
            }
            if (d2 != d1 && d2 % 5 == 2) {
                sum += (seq_t)d2 + 3;
            }
        }
    }
    return sum / 5;
}
