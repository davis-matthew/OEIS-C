/*
 * A284761  a(n) = gcd(A279513(n), A279513(n+1)).
 * https://oeis.org/A284761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( product_{p|n} (p * exponent_of_p_in_n), *
 *            product_{q|(n+1)} (q * exponent_of_q_in_(n+1)) )
 * Range:     u8 n <= 5326, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9705 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* compute A279513(m) = prod_{p|m} (p * exponent_of_p_in_m) */
static seq_t prime_tower_factorization(idx_t m)
{
    seq_t prod = 1;
    idx_t tmp = m;

    /* handle factor 2 */
    idx_t exp2 = 0;
    while ((tmp & 1ULL) == 0 && tmp > 0) {
        exp2++;
        tmp >>= 1;
    }
    if (exp2 > 0) {
        prod *= (seq_t)2 * exp2;
    }

    /* handle odd primes */
    for (idx_t p = 3; (seq_t)p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            prod *= (seq_t)p * exp;
        }
    }

    /* remaining prime > sqrt(m) */
    if (tmp > 1) {
        prod *= (seq_t)tmp * (seq_t)1;
    }

    return prod;
}

seq_t a(idx_t n)
{
    seq_t v1 = prime_tower_factorization(n);
    seq_t v2 = prime_tower_factorization(n + 1ULL);
    return gcd(v1, v2);
}
