/*
 * A377984  a(n) = 2*sigma(n) - A003961(n), where sigma is the sum of divisors
 *          function and A003961 is fully multiplicative function with
 *          a(prime(i)) = prime(i+1).
 * https://oeis.org/A377984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*sigma(n) - A003961(n) where sigma(n) = sum of divisors
 *            of n, and A003961 is the fully multiplicative function with
 *            A003961(p^e) = next_prime(p)^e for any prime p
 * Range:     i8 n <= 47, i16 n <= 1023, i32 n <= 9958, i64 n <= 9958,
 *            i128 n <= 9958, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset safety, ignored by OEIS */
    seq_t sigma = 1;               /* multiplicative sigma */
    seq_t a003961 = 1;             /* multiplicative A003961 */

    uint64_t temp = (uint64_t)n;
    uint64_t p = 2;

    while (p * p <= temp) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            /* contribution to sigma: (p^(exp+1)-1)/(p-1) */
            seq_t p_pow = 1;
            for (int i = 0; i < exp + 1; i++) p_pow *= (seq_t)p;
            seq_t sigma_term = (p_pow - 1) / ((seq_t)p - 1);
            sigma *= sigma_term;

            /* contribution to A003961: next_prime(p)^exp */
            seq_t np = (seq_t)next_prime(p);
            seq_t np_pow = 1;
            for (int i = 0; i < exp; i++) np_pow *= np;
            a003961 *= np_pow;
        }
        p = (p == 2) ? 3 : next_prime(p);
    }

    if (temp > 1) {                /* remaining prime factor */
        uint64_t p = temp;         /* exponent = 1 */
        seq_t sigma_term = (seq_t)(p + 1);  /* (p^2-1)/(p-1) = p+1 */
        sigma *= sigma_term;

        seq_t np = (seq_t)next_prime(p);
        a003961 *= np;
    }

    return (sigma * 2) - a003961;
}
