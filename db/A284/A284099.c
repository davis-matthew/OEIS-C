/*
 * A284099  a(n) = Sum_{d|n, d == 1 (mod 7)} d.
 * https://oeis.org/A284099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d mod 7 == 1} d
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of all divisors of n that are congruent to 1 modulo 7. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum; /* fallback, though offset starts at 1 */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* d is a divisor */
            if (d % 7 == 1) sum += (seq_t)d;
            /* partner divisor n/d */
            idx_t other = n / d;
            if (other != d && other % 7 == 1) sum += (seq_t)other;
        }
    }
    return sum;
}
