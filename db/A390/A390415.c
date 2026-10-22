/*
 * A390415  a(n) = Sum_{k=0..n} binomial(4*n+3*k+1,n-k).
 * https://oeis.org/A390415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} binomial(4*n+3*k+1, n-k)
 *
 * This implementation computes the binomial coefficients by prime
 * factorisation to avoid overflow on 64‑bit builds.  The intermediate
 * multiplication stays within 64 bits for all n whose result fits in
 * seq_t, thus giving correct values whenever they should.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* cached prime list for factorisation */
static uint64_t *prime_cache = NULL;
static idx_t prime_cache_max = 0;
static size_t prime_cache_cnt = 0;

/* compute binomial(n,k) with prime factorisation */
static seq_t _binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return 1;

    /* ensure prime_cache covers [2..n] */
    if (prime_cache_max < n) {
        free(prime_cache);
        build_primes_upto(n, &prime_cache, &prime_cache_cnt);
        prime_cache_max = n;
    }

    seq_t result = 1;
    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > n) break;
        idx_t exp = 0;
        idx_t power = p;
        while (power <= n) {
            exp += n / power;
            exp -= k / power;
            exp -= (n - k) / power;
            if (power > n / p) break;
            power *= p;
        }
        for (idx_t e = 0; e < exp; ++e)
            result *= (seq_t)p;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t N = 4 * n + 3 * k + 1;   /* ≥0 and ≤ 7*n+1   */
        idx_t K = n - k;
        sum += _binomial(N, K);
    }
    return sum;
}
