/*
 * A397106  a(n) = n + A275314(n), for n >= 1.
 * https://oeis.org/A397106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = n + 1 + A001414(n) - A001222(n) * A001414(n) = sum of
 *            prime factors of n (with multiplicity) * A001222(n) = Ω(n),
 *            total number of prime factors of n (with multiplicity) *
 *            Therefore a(n) = n + 1 + (sum of prime factors of n) - Ω(n) *
 *            for all n ≥ 1
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Treat n as a seq_t for arithmetic */
    seq_t val = (seq_t)n;
    seq_t sum_pf = 0;   /* sum of prime factors (with multiplicity)   */
    seq_t omega = 0;    /* total number of prime factors (with multiplicity) */

    /* Break down val into prime factors */
    for (seq_t p = 2; p * p <= val; ++p) {
        if (val % p == 0) {
            while (val % p == 0) {
                val /= p;
                sum_pf += p;
                omega++;
            }
        }
    }
    /* If remaining val > 1, it is a prime factor larger than sqrt(original n) */
    if (val > 1) {
        sum_pf += val;
        omega++;
    }

    /* Compute a(n) */
    return (seq_t)n + 1 + sum_pf - omega;
}
