/*
 * A395543  a(n) = denominator((n-1)!/n^n).
 * https://oeis.org/A395543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((n-1)!/n^n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n^n / gcd((n-1)!, n^n) */
/* Compute as: for each prime p|n with exponent e, exponent in a(n)
   = e*n - sum_{i>=1} floor((n-1)/p^i) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/pow_uint.h"            /* pow_uint */
#include "../utils/seq_mul.h"            /* seq_mul */

/* Caches for prime sieve */
static uint64_t *cached_primes = NULL;
static size_t cached_prime_count = 0;
static uint64_t cached_prime_limit = 0;

static void ensure_primes_upto(idx_t n) {
    if (n <= cached_prime_limit) return;
    /* free old cache */
    if (cached_primes) free(cached_primes);
    /* build new cache */
    build_primes_upto(n, &cached_primes, &cached_prime_count);
    cached_prime_limit = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;                 /* not used, but safe */
    if (n == 1) return (seq_t)1;                /* a(1)=1 */
    ensure_primes_upto(n);
    seq_t result = (seq_t)1;                    /* start with 1 */
    uint64_t nn = (uint64_t)n;
    /* factor n using cached primes */
    for (size_t i = 0; i < cached_prime_count; ++i) {
        uint64_t p = cached_primes[i];
        if (p * p > nn) break;
        if (nn % p != 0) continue;
        uint64_t e = 0;
        while (nn % p == 0) {
            nn /= p;
            ++e;
        }
        /* compute S = sum floor((n-1)/p^k) */
        uint64_t S = 0;
        uint64_t d = p;
        while (d <= n-1) {
            S += (n-1) / d;
            if (d > (n-1) / p) break;   /* avoid overflow */
            d *= p;
        }
        idx_t exp = (idx_t)(e * n - S);
        if (exp > 0) {
            seq_t term = pow_uint(p, exp);
            seq_t new_res = seq_mul(result, term);
            /* free intermediate if necessary? not required by spec */
            result = new_res;
        }
    }
    /* remaining prime factor, if any */
    if (nn > 1) {
        uint64_t p = nn;
        uint64_t e = 1;
        uint64_t S = 0;
        uint64_t d = p;
        while (d <= n-1) {
            S += (n-1) / d;
            if (d > (n-1) / p) break;
            d *= p;
        }
        idx_t exp = (idx_t)(e * n - S);
        if (exp > 0) {
            seq_t term = pow_uint(p, exp);
            seq_t new_res = seq_mul(result, term);
            result = new_res;
        }
    }
    return result;
}
