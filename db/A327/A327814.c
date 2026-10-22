/*
 * A327814  Number of irreducible factors in the factorization of the n-th
 *          cyclotomic polynomial over GF(5) (counted with multiplicity).
 * https://oeis.org/A327814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n)/ord(5,s), where n = 5^e * s, gcd(5,s)=1, phi is
 *            Euler's totient and ord(5,s) is the multiplicative order of 5
 *            modulo s
 * Range:     u8 n <= 623, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/pow_uint.h"

static seq_t phi_of_5_pow(uint64_t e)
{
    /* phi(5^e) = 5^e - 5^(e-1) = 5^(e-1)*4, for e>0; phi(1)=1 for e==0. */
    if (e == 0)
        return (seq_t)1;
    seq_t pow5 = pow_uint(5, e);          /* 5^e */
    seq_t pow5_div5 = pow5 / (seq_t)5;    /* 5^(e-1) */
    return pow5 - pow5_div5;              /* 4*5^(e-1) */
}

static void distinct_prime_factors(uint64_t n, uint64_t *factors, size_t *cnt)
{
    /* fills 'factors' with distinct prime factors of n, returns count in *cnt */
    *cnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
        if (tmp % p == 0) {
            factors[(*cnt)++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    if (tmp > 1)
        factors[(*cnt)++] = tmp;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* not part of OEIS offset, harmless */

    /* split n = 5^e * s, gcd(5,s)=1 */
    uint64_t m = (uint64_t)n;
    uint64_t e = 0;
    while (m % 5 == 0) {
        m /= 5;
        ++e;
    }
    uint64_t s = m;  /* coprime to 5 */

    /* compute phi(s) as uint64_t (s <= 1e4, phi(s) fits in 64 bits) */
    uint64_t phi_s = s;
    uint64_t tmp = s;
    for (uint64_t p = 2; p * p <= tmp; ++p)
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            phi_s -= phi_s / p;
        }
    if (tmp > 1)
        phi_s -= phi_s / tmp;

    /* phi(5^e) */
    seq_t phi_5e = phi_of_5_pow(e);

    /* phi(n) = phi(5^e) * phi(s) */
    seq_t phi_n = phi_5e * (seq_t)phi_s;

    /* order of 5 modulo s */
    uint64_t ord = 1;
    if (s == 1) {
        ord = 1;
    } else {
        uint64_t phi_val = phi_s;            /* integer phi(s) */
        uint64_t factors[64], fcnt;
        distinct_prime_factors(phi_val, factors, &fcnt);
        ord = phi_val;
        for (uint64_t i = 0; i < fcnt; ++i) {
            uint64_t p = factors[i];
            while (ord % p == 0) {
                uint64_t candidate = ord / p;
                if (mod_pow((seq_t)5, candidate, (seq_t)s) == (seq_t)1)
                    ord = candidate;
                else
                    break;
            }
        }
    }

    /* compute a(n) = phi(n) / ord */
    return phi_n / (seq_t)ord;
}
