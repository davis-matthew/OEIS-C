/*
 * A367379  a(n) = Sum_{j=1..n} Sum_{i=1..n} (j mod i).
 * https://oeis.org/A367379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} Sum_{i=1..n} (j mod i)
 * Range:     u8 n <= 10, u16 n <= 66, u32 n <= 2669, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t q = n / i;          /* number of complete i‑cycles   */
        idx_t r = n % i;          /* remainder after complete cycles */

        /* Sum of remainders in each complete cycle: 1 + 2 + … + (i-1) = i*(i-1)/2 */
        seq_t cycle_sum = (seq_t)i * ((seq_t)i - 1) / 2;
        seq_t term1 = (seq_t)q * cycle_sum;

        /* Sum of remainders in the final incomplete cycle: 1 + 2 + … + r = r*(r+1)/2 */
        seq_t term2 = (seq_t)r * ((seq_t)r + 1) / 2;

        total += term1 + term2;
    }
    return total;
}
