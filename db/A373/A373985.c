/*
 * A373985  a(n) = gcd(A108951(n), A373158(n)), where A108951 is fully
 *          multiplicative and A373158 is fully additive with a(p) = p# for
 *          prime p, where x# is the primorial A034386(x).
 * https://oeis.org/A373985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A108951(n), A373158(n)) where A108951 is fully
 *            multiplicative with A108951(p) = p# (primorial), and A373158 is
 *            fully additive with A373158(p) = p#
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Computes a(n) as defined above for all n >= 1.  The function
   works independently of any calling order and uses only the
   provided C/Cpp helpers.  */
seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1ULL) {
        return (seq_t)1;
    }

    /* 1. factorisation of n */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t   fc = 0;
    factor_u64((uint64_t)n, pf, ex, &fc);

    /* 2. find maximum prime divisor of n */
    uint64_t maxPrime = 0;
    for (size_t i = 0; i < fc; ++i) {
        if (pf[i] > maxPrime) {
            maxPrime = pf[i];
        }
    }

    /* 3. list of all primes <= maxPrime (used for primorials) */
    uint64_t *primes = NULL;
    size_t    primeCnt = 0;
    if (maxPrime > 0) {
        build_primes_upto(maxPrime, &primes, &primeCnt);
    } else {
        primeCnt = 0;
    }

    /* 4. result accumulator */
    seq_t result = (seq_t)1;

    /* 5. process each prime q <= maxPrime */
    for (size_t qi = 0; qi < primeCnt; ++qi) {
        uint64_t q = primes[qi];

        /* exponent of q in A108951(n) */
        int exp_f = 0;
        for (size_t fi = 0; fi < fc; ++fi) {
            if (pf[fi] >= q) {
                exp_f += (int)ex[fi];
            }
        }
        if (exp_f == 0) {
            continue;   /* q does not divide A108951(n) */
        }

        /* exponent of q in A373158(n): v_q(S) where
           S = Σ ex[i] * p#   (p = pf[i])  */
        seq_t mod = (seq_t)q;
        int exp_g = 0;

        while (true) {
            /* compute S modulo mod */
            seq_t sum = 0;
            for (size_t fi = 0; fi < fc; ++fi) {
                uint64_t p = pf[fi];
                /* primorial of p modulo mod */
                seq_t prim = (seq_t)1;
                for (size_t pj = 0; pj < primeCnt && primes[pj] <= p; ++pj) {
                    prim = (prim * (seq_t)primes[pj]) % mod;
                }
                seq_t term = ((seq_t)ex[fi] % mod) * prim % mod;
                sum += term;
                sum %= mod;
            }

            /* if S mod mod != 0, stop.  otherwise increase exponent */
            if (sum % mod != 0) {
                break;
            }
            exp_g++;
            if (exp_g >= exp_f) {
                break;   /* we already have enough for the gcd */
            }
            mod *= (seq_t)q;   /* next power of q */
        }

        int exponent = (exp_f < exp_g) ? exp_f : exp_g;
        if (exponent > 0) {
            seq_t pow_val = seq_pow((seq_t)q, (idx_t)exponent);
            result = seq_mul(result, pow_val);
        }
    }

    /* clean up */
    free(primes);
    return result;
}
