/*
 * A380541  a(n) is the multiplicative order of -8 modulo prime(n); a(1) = 0
 *          for completion.
 * https://oeis.org/A380541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multiplicative order of -8 modulo prime(n); a(1)=0
 * Range:     u8 n <= 54, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the property that the order divides p-1 and factors are removed
   by testing with modular exponentiation.  The routine is
   dedicated to the OEIS sequence A380541. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* Prime sieve cache for factorisation of p-1 (p <= prime(10000)=104729)  */
/* ------------------------------------------------------------------ */
static uint64_t *prime_cache = NULL;
static size_t     prime_cache_cnt = 0;
static uint64_t   prime_cache_limit = 0;

/* Ensure that the sieve cache contains all primes up to `limit`. */
static void ensure_prime_cache(uint64_t limit)
{
    if (limit <= prime_cache_limit)
        return;
    /* enlarge or build anew */
    if (prime_cache)
        free(prime_cache);
    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Compute multiplicative order of -8 modulo the given prime (p>=3).   */
/* ------------------------------------------------------------------ */
static seq_t order_minus8(uint32_t prime)
{
    const seq_t one = (seq_t)1;

    /* -8 mod p, with correct positive residue */
    int64_t tmp = -8 % (int64_t)prime;
    if (tmp < 0)
        tmp += prime;
    const seq_t base = (seq_t)tmp;

    /* order must divide p-1 */
    uint64_t order_ll = (uint64_t)prime - 1;

    /* Factorise order_ll into distinct prime factors */
    ensure_prime_cache(65536);            /* enough for all n <= 10000 */
    uint64_t ord_base = order_ll;         /* copy for factorisation */
    uint64_t factor[32];
    size_t   num_factor = 0;

    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t q = prime_cache[i];
        if (q * q > ord_base)
            break;
        if (ord_base % q == 0) {
            factor[num_factor++] = q;
            while (ord_base % q == 0)
                ord_base /= q;
        }
    }
    if (ord_base > 1)                 /* remaining prime factor */
        factor[num_factor++] = ord_base;

    /* Reduce the order by checking each factor */
    for (size_t i = 0; i < num_factor; ++i) {
        uint64_t q = factor[i];
        while (order_ll % q == 0) {
            seq_t test = mod_pow(base, order_ll / q, (seq_t)prime);
            if (test == one)
                order_ll /= q;
            else
                break;
        }
    }

    return (seq_t)order_ll;
}

/* ------------------------------------------------------------------ */
/* Public entry point for the sequence.                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)                       /* a(1) = 0 for p = 2 */
        return (seq_t)0;

    uint32_t p32 = get_prime(n);     /* nth prime (unsigned 32-bit) */
    if (p32 == 2)                    /* safety check */
        return (seq_t)0;

    return order_minus8(p32);
}
