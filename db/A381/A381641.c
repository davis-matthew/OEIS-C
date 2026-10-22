/*
 * A381641  Numerators of Sum_{i=1..omega(n)-1} (1 - p_{i}/p_{i+1}), where
 *          omega(n) = A001221(n) and p_1 < p_2 < ... p_omega(n) are the
 *          distinct prime factors of n; a(1) = 0.
 * https://oeis.org/A381641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = Numerator of Σ_{i=1}^{ω(n)-1} (1 - p_i/p_{i+1}), where
 *            p_1 < p_2 < … < p_{ω(n)} are the distinct prime factors of n.
 *            a(1) = 0
 * Range:     u8 n <= 369, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;  /* a(1) = 0 */

    uint64_t temp = n;
    uint64_t primes[64];
    int pc = 0;

    for (uint64_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            primes[pc++] = i;
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) primes[pc++] = temp;

    if (pc < 2) return 0;   /* prime power or 1 */

    seq_t num = 0;   /* numerator of running sum */
    seq_t den = 1;   /* denominator of running sum */

    for (int i = 0; i < pc - 1; ++i) {
        seq_t pi_next = primes[i + 1];
        seq_t pi_now  = primes[i];
        seq_t term_num = pi_next - pi_now;
        seq_t term_den = pi_next;

        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;

        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;  /* numerator after reduction */
}
