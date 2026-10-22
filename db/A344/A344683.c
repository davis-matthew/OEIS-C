/*
 * A344683  Dirichlet convolution of the Euler totient function with itself,
 *          applied twice.
 * https://oeis.org/A344683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative with a(p^e) = (1/2)*(p-1)*p^(e-3)*(e²*(p-1)² +
 *            3e*(p²-1) + 2*(p²+p+1))
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 2370, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Compute a(p^e) for prime p (uint64_t) and exponent e (idx_t) */
static seq_t a_prime_power(uint64_t p, idx_t e)
{
    seq_t seq_p = (seq_t)p;
    seq_t seq_e = (seq_t)e;
    seq_t p1   = seq_p - (seq_t)1;           /* p-1 */
    seq_t e2   = seq_e * seq_e;              /* e² */
    seq_t p2   = seq_p * seq_p;              /* p² */

    /* bracket = e²*(p-1)² + 3e*(p²-1) + 2*(p²+p+1) */
    seq_t bracket = e2 * p1 * p1
                  + (seq_t)3 * seq_e * (p2 - (seq_t)1)
                  + (seq_t)2 * (p2 + seq_p + (seq_t)1);

    if (e >= 3) {
        /* p^(e-3) with e>=3 */
        seq_t p_pow = pow_uint((uint64_t)seq_p, e - 3);
        seq_t product = p1 * p_pow;
        product *= bracket;
        return product / (seq_t)2;
    } else {
        /* e<3: divide by 2*p^(3-e) */
        idx_t diff = 3 - e;                  /* 3-e is positive here */
        seq_t denom = (seq_t)2 * pow_uint((uint64_t)seq_p, diff);
        seq_t product = p1 * bracket;
        return product / denom;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* factor n using the supplied utility */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= a_prime_power(primes[i], (idx_t)exps[i]);
    }
    return result;
}
