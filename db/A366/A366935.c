/*
 * A366935  Moduli k for which the number of quadratic residues mod k coprime
 *          to k is equal to phi(k)/2^(phi(k)/lambda(k)), where lambda is
 *          Carmichael's function.
 * https://oeis.org/A366935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th element of OEIS sequence A366935.
   A term k is accepted if the number of quadratic residues coprime to k equals
   φ(k) / 2^(φ(k)/λ(k)), where φ is Euler's totient and λ is Carmichael's
   function.  The test is performed by prime factorisation and computing
   φ(k), λ(k) and the exponent r of the powers of two required for the
   equality. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/gcd.h"           /* seq_t gcd(seq_t, seq_t) */
#include "../utils/pow2.h"          /* seq_t pow2(idx_t)          */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/phi.h"           /* seq_t phi(idx_t)          */   /* optional, not used */

static inline seq_t pow_uint64(uint64_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    for (unsigned int i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

/* Test whether the modulus k satisfies the OEIS condition. */
static bool is_good_k(uint64_t k)
{
    /* Factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    seq_t phi = (seq_t)1;   /* φ(k) */
    seq_t lam = (seq_t)1;   /* λ(k) as lcm of prime-powers' λ */

    size_t odd_prime_cnt = 0;  /* number of odd prime factors */
    unsigned int exp2 = 0;     /* exponent of 2 in k */

    /* Compute φ(k) and λ(k) from factorisation */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned int exp = (unsigned int)exps[i];

        if (p == 2) {
            exp2 = exp;
            /* λ(2^exp) */
            seq_t lam_i;
            if (exp == 1)
                lam_i = (seq_t)1;
            else if (exp == 2)
                lam_i = (seq_t)2;
            else
                lam_i = pow2(exp - 2);        /* 2^(exp-2) */
            /* φ(2^exp) = 2^(exp-1) */
            seq_t phi_mult = pow_uint64(2, exp - 1);
            phi *= phi_mult;
            /* lcm step */
            seq_t g = gcd(lam, lam_i);
            lam = lam / g * lam_i;
        }
        else {
            odd_prime_cnt++;
            /* φ(p^exp) = p^(exp-1)*(p-1) */
            seq_t p_pow = pow_uint64(p, exp - 1);
            seq_t phi_mult = p_pow * ((seq_t)p - 1);
            phi *= phi_mult;
            /* λ(p^exp) = φ(p^exp) for odd primes */
            seq_t lam_i = phi_mult;
            seq_t g = gcd(lam, lam_i);
            lam = lam / g * lam_i;
        }
    }

    /* Compute the exponent r of the powers of two in |Q_k| */
    size_t r = odd_prime_cnt;
    if (exp2 == 2)
        r += 1;
    else if (exp2 >= 3)
        r += 2;          /* exp2 == 0 or 1 contribute 0 */

    /* φ(k)/λ(k) must be integer; compute it */
    seq_t phi_over_lam = phi / lam;

    return phi_over_lam == (seq_t)r;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0 */

    idx_t count = 0;
    uint64_t k = 3;                /* first term in the sequence */

    while (true) {
        if (is_good_k(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
