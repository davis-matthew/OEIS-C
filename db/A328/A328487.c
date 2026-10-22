/*
 * A328487  Dirichlet g.f.: zeta(s)^2 * zeta(s-1)^2 * (1 - 2^(1 - s))^2.
 * https://oeis.org/A328487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} A000593(d) * A000593(n/d)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 693, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a is the Dirichlet convolution of A000593 with itself.  */
/* This implementation uses the multiplicative formula: */

/*  a(2^e) = e + 1
    a(p^e) = ((e+1)*p^(e+3) - (e+3)*(p^(e+2) - p + 1) + 2)/(p-1)^3   (p odd) */

#include <stdint.h>
#include <stddef.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static seq_t a_prime_power(uint64_t p, idx_t e)
{
    if (p == 2) {
        return (seq_t)(e + 1);
    } else {
        seq_t p_pow_e_plus3 = pow_seq((seq_t)p, e + 3);
        seq_t p_pow_e_plus2 = pow_seq((seq_t)p, e + 2);
        seq_t term1 = (seq_t)(e + 1) * p_pow_e_plus3;
        seq_t term2 = (seq_t)(e + 3) * (p_pow_e_plus2 - (seq_t)p + (seq_t)1);
        seq_t numerator = term1 - term2 + (seq_t)2;
        seq_t denominator = (seq_t)(p - 1);
        denominator *= denominator;
        denominator *= (seq_t)(p - 1);
        return numerator / denominator;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t m = n;
    seq_t result = (seq_t)1;

    for (idx_t p = 2; p * p <= n; ++p) {   /* p ranges over potential primes */
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= a_prime_power((uint64_t)p, exp);
        }
    }

    if (m > 1) {   /* remaining prime factor */
        result *= a_prime_power(m, (idx_t)1);
    }

    return result;
}
