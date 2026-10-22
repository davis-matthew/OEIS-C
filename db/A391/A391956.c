/*
 * A391956  a(n) = 1/Product_{i = 1..n} w_i, where w_i is the i-th weight
 *          factor for Laguerre-Gauss quadrature of degree n.
 * https://oeis.org/A391956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} k^(4*k - 2*n - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) */
#include "../utils/pow_uint.h"           /* seq_t pow_uint(uint64_t base, idx_t exp) */

/* Cached list of primes up to the last requested limit. */
static uint64_t *prime_cache = NULL;
static size_t    prime_count = 0;
static idx_t     prime_limit = 0;

/* Ensure that prime_cache contains all primes ≤ n.  Free the old cache if it becomes
   insufficient. */
static void ensure_primes_upto(idx_t n)
{
    if (n <= prime_limit) return;
    if (prime_cache) free(prime_cache);
    build_primes_upto(n, &prime_cache, &prime_count);
    prime_limit = n;
}

/* Main sequence function.  It works with any seq_t type (fixed‑size or big integer). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_primes_upto(n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_cache[i];

        /* weighted_sum = Σ_{k=1}^{n} k * v_p(k) */
        uint64_t weighted_sum = 0;
        uint64_t power = p;
        while (power <= (uint64_t)n) {
            uint64_t t = ((uint64_t)n) / power;
            /* contribution of this power layer: power * t*(t+1)/2 */
            uint64_t contrib = power * (t * (t + 1) / 2);
            weighted_sum += contrib;
            if (p != 0 && power > ((uint64_t)n) / p) break;
            power *= p;
        }

        /* exponent of p in n!  (Legendre's formula) */
        uint64_t fexpo = 0;
        uint64_t power_f = p;
        while (power_f <= (uint64_t)n) {
            fexpo += ((uint64_t)n) / power_f;
            if (p != 0 && power_f > ((uint64_t)n) / p) break;
            power_f *= p;
        }

        /* E(p) = 4*weighted_sum - (2*n+1)*fexpo  (always non‑negative) */
        __int128 diff = (__int128)4 * (int64_t)weighted_sum -
                        (__int128)(2ULL * (uint64_t)n + 1ULL) * (int64_t)fexpo;
        uint64_t expo = (diff > 0) ? (uint64_t)diff : 0;

        if (expo > 0) {
            seq_t term = pow_uint(p, expo);
            result *= term;           /* multiplication is allowed on seq_t */
        }
    }
    return result;
}
