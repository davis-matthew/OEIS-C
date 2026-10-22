/*
 * A351168  If n = p_1^e_1 * ... * p_k^e_k, where p_1 < ... < p_k are primes,
 *          then a(n) is obtained by replacing the last factor p_k^e_k by (p_k
 *          - 1)^e_k; a(1) = 1.
 * https://oeis.org/A351168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(1 - 1/p_k)^e_k where p_k is the largest prime factor
 *            of n and e_k its exponent
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t seq_pow(seq_t base, unsigned int exp) {
    seq_t result = (seq_t)1;
    while (exp--) {
        result *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)1;
    }

    uint64_t original = (uint64_t)n;
    uint64_t temp = original;
    uint64_t last_prime = 0;
    unsigned int last_exp   = 0;

    /* Simple prime factorisation: find the largest prime factor and its exponent. */
    while (temp > 1) {
        uint64_t pf = 2;
        if (temp % 2 == 0) {
            pf = 2;
        } else {
            for (uint64_t i = 3; i * i <= temp; i += 2) {
                if (temp % i == 0) {
                    pf = i;
                    break;
                }
            }
            if (pf == 2) { /* never reached, placeholder */
                pf = temp;
            }
        }
        /* If no divisor found up to sqrt(temp), temp itself is prime. */
        if (pf == 0) {
            pf = temp;
        }

        unsigned int exp = 0;
        while (temp % pf == 0) {
            temp /= pf;
            exp++;
        }
        last_prime = pf;
        last_exp   = exp;
    }

    /* Compute n / p^e * (p-1)^e. */
    seq_t p_pow   = seq_pow((seq_t)last_prime, last_exp);
    seq_t other   = (seq_t)original / p_pow;
    seq_t mult    = seq_pow((seq_t)(last_prime - 1), last_exp);
    return other * mult;
}
