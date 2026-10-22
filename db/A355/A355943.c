/*
 * A355943  a(n) = 1 if n is odd and A064989(sigma(n)) divides A064989(n),
 *          otherwise 0, where A064989 is fully multiplicative with a(2) = 1
 *          and a(p) = prevprime(p) for odd primes p, and sigma is the sum of
 *          divisors function.
 * https://oeis.org/A355943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and A064989(sigma(n)) divides A064989(n), *
 *            otherwise 0. A064989 is multiplicative with a(2)=1 and
 *            a(p)=prevprime(p) * for odd primes p. Sigma is sum of divisors
 * Range:     u8 n <= 16, u16 n <= 1698, u32 n <= 9768, u64 n <= 9768,
 *            u128 n <= 9768, big any n
 * Helpers:   big_pow, build_primes_upto, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a small sieve to factor numbers up to a fixed limit.
 * For n up to about 10000, the sieve size of 200000 covers all needed
 * primes for factoring n and sigma(n).  All operations are performed
 * with the generic seq_t type, which may be an unsigned integer type
 * or an arbitrary-precision integer. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"          /* seq_t big_pow(seq_t, seq_t) */
#include "../utils/prev_prime.h"       /* idx_t prev_prime(idx_t) */
#include "../utils/build_primes_upto.h"/* void build_primes_upto(idx_t, uint64_t **, size_t *) */

static uint64_t *prime_list = NULL;    /* sieve primes */
static size_t prime_count = 0;
static bool primes_ready = false;
#define PRIME_LIMIT 200000              /* enough for n <= 10000 */

/* initialise the prime list once */
static void init_primes(void)
{
    if (!primes_ready) {
        build_primes_upto((idx_t)PRIME_LIMIT, &prime_list, &prime_count);
        primes_ready = true;
    }
}

/* calculate the multiplicative value A064989 for a uint64_t n */
static seq_t a064989_uint64(uint64_t n)
{
    seq_t prod = 1;
    uint64_t m = n;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p != 2) {
                seq_t prev = (seq_t)prev_prime(p);
                prod *= big_pow(prev, (seq_t)e);
            }
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        if (m != 2) {
            seq_t prev = (seq_t)prev_prime(m);
            prod *= prev;         /* exponent is 1 */
        }
    }
    return prod;
}

/* calculate the multiplicative value A064989 for a seq_t number that we know
 * fits in uint64_t (used for sigma(n) in the range this routine supports). */
static seq_t a064989_uint64_from_seq(seq_t val)
{
    /* val is guaranteed to fit in 64 bits in the used range */
    uint64_t v = (uint64_t)val;
    return a064989_uint64(v);
}

/* compute sigma(n) (sum of divisors) as seq_t, using factorisation of n */
static seq_t sigma_uint64_to_seq(uint64_t n)
{
    seq_t sigma = 1;
    uint64_t m = n;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t p_power = big_pow((seq_t)p, (seq_t)(e + 1));
            seq_t denom = (seq_t)(p - 1);
            seq_t factor = (p_power - 1) / denom;
            sigma *= factor;
        }
    }
    if (m > 1) {     /* remaining prime factor */
        seq_t p_power = big_pow((seq_t)m, 2);
        seq_t denom = (seq_t)(m - 1);
        seq_t factor = (p_power - 1) / denom;
        sigma *= factor;
    }
    return sigma;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_primes();                        /* ensure primes are ready */
    if ((n & 1) == 0)                    /* even n → 0 */
        return 0;

    /* --- compute A064989(n) --- */
    seq_t a_n = a064989_uint64((uint64_t)n);

    /* --- compute sigma(n) --- */
    seq_t sigma_val = sigma_uint64_to_seq((uint64_t)n);

    /* sigma_val must fit in 64-bit for this routine (valid for n ≤ 10000) */
    if (sigma_val > (seq_t)(UINT64_MAX))
        return 0;                /* fallback: cannot handle huge sigma */

    uint64_t sigma_u = (uint64_t)sigma_val;

    /* --- compute A064989(sigma(n)) --- */
    seq_t a_sigma = a064989_uint64(sigma_u);

    /* --- check divisibility --- */
    if (a_n % a_sigma == 0)
        return 1;
    else
        return 0;
}
