/*
 * A397140  a(n) = number of positive g < prime(n) such that g and g+1 are
 *          both primitive roots modulo prime(n).
 * https://oeis.org/A397140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6576, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   factorize, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A397140
    Formula: a(n) = |{ 1 ≤ g < prime(n) : g and g+1 are primitive roots (mod prime(n)) }|
    The implementation finds one primitive root of each prime p, builds a
    table of all primitive roots, then counts consecutive pairs in that
    table.  Complexity is O(p) per prime, well below the limits for n up to
    10000. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* utilities provided by the build system */
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t) */
#include "../utils/factorize.h"     /* size_t factorize(uint64_t n, uint64_t *primes, int *exps) */
#include "../utils/mod_pow.h"       /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

/* Euclidean GCD for two 64‑bit values */
static inline uint64_t _gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Find a primitive root of prime p.  p is a 32‑bit unsigned integer. */
static uint64_t _primitive_root(uint32_t p, const uint64_t *factors, size_t fac_cnt)
{
    if (p == 2) return 0;            /* no primitive roots < p */
    uint64_t exp;

    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < fac_cnt && ok; ++i) {
            exp = (p - 1) / factors[i];
            if (mod_pow((seq_t)g, exp, (seq_t)p) == (seq_t)1)
                ok = false;
        }
        if (ok) return g;
    }
    return 0;                         /* should never happen for primes > 2 */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint32_t pr = get_prime(n);          /* prime(p) */
    uint64_t p = (uint64_t)pr;
    uint64_t pm1 = p - 1;

    /* factorise p-1 */
    uint64_t fac[64];
    int exps[64];                       /* exps not needed, but required by API */
    size_t fac_cnt = factorize(pm1, fac, exps);

    /* find one primitive root g0 */
    uint64_t g0 = _primitive_root((uint32_t)p, fac, fac_cnt);

    /* table of primitive‑root flags */
    uint8_t *isPrim = (uint8_t *)calloc((size_t)p, sizeof(uint8_t));
    if (!isPrim) return (seq_t)0;       /* allocation failure → zero */

    /* walk powers g0^k (k = 1..p-1) and mark primitives */
    uint64_t power = 1;
    for (uint64_t k = 1; k < p; ++k) {
        power = (power * g0) % p;           /* next power */
        if (_gcd64(k, pm1) == 1)
            isPrim[power] = 1;
    }

    /* count consecutive primitive roots */
    seq_t count = 0;
    for (uint64_t g = 1; g < pm1; ++g) {
        if (isPrim[g] && isPrim[g + 1])
            ++count;
    }

    free(isPrim);
    return count;
}
