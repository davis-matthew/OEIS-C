/*
 * A397890  Minimum binary length of a standard-order composition with n
 *          distinct runs; bit length of A351015(n).
 * https://oeis.org/A397890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0 * a(n) = Sum_{j=1..n} A061017(j) for n >= 1 * Where
 *            A061017(j) is the smallest integer q with D(q-1) < j <= D(q), *
 *            and D(q) = Sum_{k=1..q} d(k) (divisor summatory function). * *
 *            This implementation computes a(n) directly by accumulating the *
 *            divisor counts (d) and the weighted sums m...
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t q = 1;           /* current integer whose divisor count is needed */
    idx_t cumulative_D = 0;/* D(q-1) */
    seq_t cumulative_S = 0;/* Sum_{m<q} m*d(m) */

    while (1) {
        /* Compute d(q) = number of divisors of q */
        idx_t t = q;
        unsigned int d = 1;
        for (idx_t p = 2; p * p <= t; ++p) {
            if (t % p == 0) {
                unsigned int exp = 0;
                while (t % p == 0) {
                    t /= p;
                    ++exp;
                }
                d *= (exp + 1);
            }
        }
        if (t > 1)  /* remaining prime factor */
            d *= 2;

        idx_t new_D = cumulative_D + (idx_t)d;
        if (new_D >= n) {
            /* n lies in the interval (D(q-1), D(q)] */
            return cumulative_S + (seq_t)q * ((seq_t)n - (seq_t)cumulative_D);
        }

        /* Update running totals and move to next q */
        cumulative_D = new_D;
        cumulative_S += (seq_t)q * (seq_t)d;
        ++q;
    }
}
