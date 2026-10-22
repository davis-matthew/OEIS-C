/*
 * A373362  a(n) = gcd(A001414(n), A276085(n)), where A001414 is the sum of
 *          prime factors with repetition, and A276085 is the primorial base
 *          log-function.
 * https://oeis.org/A373362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001414(n), A276085(n))
 * Range:     u8 n <= 537, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   build_primes_upto, factor_u64, gcd, mod_mul_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/mod_mul_seq_t.h"

/* static cache for primes up to the maximum n (10000) */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static int      primes_inited = 0;

/* Computes sum of prime factors (with multiplicity) in seq_t */
static seq_t sum_prime_factors(uint64_t n)
{
    seq_t sum = 0;
    uint64_t p[64], e[64];
    size_t pcnt = 0;
    factor_u64(n, p, e, &pcnt);
    for (size_t i = 0; i < pcnt; ++i) {
        sum += (seq_t)p[i] * (seq_t)e[i];
    }
    return sum;
}

/* Computes A276085(n) modulo mod = sum of prime factors */
static seq_t primorial_weight_mod(seq_t mod, uint64_t p)
{
    seq_t prod = 1;
    for (size_t j = 0; j < primes_cnt; ++j) {
        if (primes[j] >= p) break;
        prod = mod_mul_seq_t(prod, (seq_t)primes[j], mod);
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;            /* a(1) = 0 */

    /* initialise prime list once */
    if (!primes_inited) {
        build_primes_upto(10000, &primes, &primes_cnt);
        primes_inited = 1;
    }

    seq_t sum = sum_prime_factors((uint64_t)n);   /* A001414(n) */

    uint64_t p[64], e[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, p, e, &pcnt);

    seq_t tmod = 0;                 /* A276085(n) modulo sum */
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t prim = primorial_weight_mod(sum, p[i]);   /* previous-primorial mod */
        seq_t add = mod_mul_seq_t(prim, (seq_t)e[i], sum);
        tmod = (tmod + add) % sum;
    }

    return gcd(sum, tmod);          /* A373362(n) */
}
