/*
 * A349469  Dirichlet g.f.: Sum_{n>0} a(n)/n^s =
 *          zeta(s-1)*zeta(s-3)/(zeta(s-2))^2.
 * https://oeis.org/A349469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * A340850(n), multiplicative with * a(p^e) = p^e *
 *            (p^(2*e)-1) * (p-1) / (p+1) for prime p, e>0. * The
 *            implementation factors n by trial division and multiplies the *
 *            contributions from each prime power
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast integer power for seq_t */
static inline seq_t seq_pow_uint(seq_t base, uint64_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* a(p^e) for a prime power */
static inline seq_t a_prime_power(uint64_t p, uint64_t e) {
    seq_t pow_pe = seq_pow_uint((seq_t)p, e);          /* p^e */
    seq_t pow_p2e = pow_pe * pow_pe;                  /* p^(2e) */
    seq_t num = pow_pe * (pow_p2e - (seq_t)1);         /* p^e * (p^(2e)-1) */
    num *= (seq_t)(p - 1);                            /* * (p-1) */
    seq_t res = num / (seq_t)(p + 1);                 /* / (p+1) */
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;              /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    uint64_t exp;

    /* factor 2 separately */
    if ((m & 1) == 0) {
        exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            exp++;
        }
        result *= a_prime_power(2, exp);
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            result *= a_prime_power(p, exp);
        }
    }

    /* remaining prime factor > sqrt(original m) */
    if (m > 1)
        result *= a_prime_power(m, 1);

    return result;
}
