/*
 * A350072  a(n) = sigma(n^2) / gcd(sigma(n^2), A003961(n^2)), where A003961
 *          shifts the prime factorization of n one step towards larger
 *          primes, and sigma is the sum of divisors function.
 * https://oeis.org/A350072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n^2) / gcd(sigma(n^2), A003961(n^2))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1758, u64 n <= 8468,
 *            u128 n <= 8468, big any n
 * Helpers:   gcd, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */
#include "../utils/seq_pow.h"      /* seq_pow(seq_t, idx_t) */
#include "../utils/next_prime.h"   /* next_prime(uint64_t) */

static seq_t pow_seq_u64(uint64_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

static uint64_t next_prime_uint64(uint64_t p)
{
    /* next_prime is provided by the utils; fall back to simple test if
       the header does not provide it. */
    /* The utils provide an implementation named next_prime. */
    return next_prime(p);
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 explicitly (all functions work
       correctly but this avoids division by zero if the factorization
       logic were changed). */
    if (n == 1) return (seq_t)1;

    /* Factor n into primes and exponents.  For n up to ~10⁴ trial
       division is fast enough and guaranteed to finish in a fraction
       of a millisecond.  The local arrays are sized large enough to
       hold all prime factors of a 64‑bit integer. */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt]   = p;
            exps[pcnt]     = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Compute sigma(n²) and A003961(n²).  For each prime p^e
       dividing n we have:  exponent in n² = 2·e. */
    seq_t sigma  = (seq_t)1;
    seq_t af961  = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p      = primes[i];
        uint64_t e      = exps[i];
        uint64_t exp_sq = e * 2u;          /* exponent in n² */

        /* sigma part: (p^(exp_sq+1) - 1) / (p - 1) */
        seq_t p_seq          = (seq_t)p;
        seq_t numerator      = seq_pow(p_seq, (idx_t)(exp_sq + 1));
        seq_t sigma_term     = (numerator - (seq_t)1) / (p_seq - (seq_t)1);
        sigma *= sigma_term;

        /* A003961 part: (nextprime(p))^exp_sq */
        uint64_t next_p = next_prime_uint64(p);
        seq_t next_p_seq = (seq_t)next_p;
        seq_t af_term    = seq_pow(next_p_seq, (idx_t)exp_sq);
        af961 *= af_term;
    }

    seq_t g = gcd(sigma, af961);
    return sigma / g;
}
