/*
 * A363899  Expansion of Sum_{k>0} k * x^(3*k) / (1 - x^(5*k)).
 * https://oeis.org/A363899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d == 3 mod 5} d
 * Range:     u8 n <= 503, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // If n == 0 the sequence is undefined, return 0 for safety
    if (n == 0) return (seq_t)0;
    seq_t sum = 0;
    // iterate over all divisors d of n
    for (idx_t d = 1; d <= n / d; ++d) { // d <= n/d ensures we process each pair once
        if (n % d == 0) {
            idx_t q1 = n / d;            // quotient for divisor d
            if (q1 % 5 == 3) {
                sum += (seq_t)d;
            }

            idx_t d2 = q1;                // paired divisor (n/d)
            if (d2 != d) {
                idx_t q2 = d;              // quotient for divisor d2
                if (q2 % 5 == 3) {
                    sum += (seq_t)d2;
                }
            }
        }
    }
    return sum;
}
