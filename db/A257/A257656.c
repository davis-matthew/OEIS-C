/*
 * A257656  Expansion of f(x) * f(x^3) * f(-x^4)^2 * chi(-x^6)^2 in powers of
 *          x where chi(), f() are Ramanujan theta functions.
 * https://oeis.org/A257656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative, a(2^e) = -1, a(p^e) =
 *            (((p*Kronecker(12,p))^(e+1)-1)/(p*Kronecker(12,p)-1)) for odd
 *            prime p
 * Range:     i8 n <= 12, i16 n <= 1513, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9204 (2026-11-01)
 * Helpers:   build_primes_upto, ipow, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
#include "../utils/isqrt.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Global cache of primes for factorisation */
static uint64_t *prime_cache = NULL; /* array of primes up to prime_limit */
static size_t prime_count = 0;       /* number of primes stored */
static idx_t prime_limit = 0;        /* maximum prime generated so far */

/* Ensure prime_cache contains all primes up to at least 'limit' */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_cache)
    {
        free(prime_cache);
        prime_cache = NULL;
    }
    build_primes_upto(limit, &prime_cache, &prime_count);
    prime_limit = limit;
}

/* Contribution of an odd prime power p^e.
   p is odd and >=3. */
static seq_t prime_contrib(uint64_t p, unsigned int e)
{
    int kr = jacobi_symbol(12, p);        /* (12|p) in {-1,0,1} */
    seq_t base = (seq_t)p * (seq_t)kr;    /* base = p*kr  (may be negative) */
    seq_t num  = ipow(base, (seq_t)(e + 1)) - 1; /* (base)^(e+1) - 1 */
    seq_t den  = base - 1;                /* base - 1 */
    return num / den;                     /* exact division */
}

seq_t a(idx_t n)
{
    if (n == 0)             /* a(0) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* factor 2 */
    if ((n & 1) == 0)       /* any power of 2 contributes -1 */
        result *= (seq_t)(-1);

    /* strip all powers of 2 from n for the odd part */
    idx_t m = n >> __builtin_ctzll(n);

    /* factor the odd part */
    idx_t sqrt_m = isqrt(m);
    ensure_primes_upto(sqrt_m);

    for (size_t i = 0; i < prime_count && prime_cache[i] * prime_cache[i] <= m; ++i)
    {
        uint64_t p = prime_cache[i];
        if (m % p == 0)
        {
            unsigned int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result *= prime_contrib(p, e);
        }
    }

    /* any remaining factor >1 is a prime > sqrt(original n) */
    if (m > 1)
        result *= prime_contrib(m, 1);

    return result;
}
