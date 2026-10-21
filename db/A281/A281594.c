/*
 * A281594  The radical of the Catalan number which is the largest squarefree
 *          number dividing binomial(2*n,n)/(n+1).
 * https://oeis.org/A281594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = radical( binomial(2*n, n) / (n+1) )
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"  /* build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) */
#include "../utils/seq_mul.h"           /* seq_mul(seq_t a, seq_t b) */

static uint64_t *cached_primes = NULL;
static size_t cached_prime_count = 0;
static idx_t cached_prime_limit = 0;

/* Legendre exponent of prime p in m! */
static inline idx_t legendre_exponent(idx_t m, uint64_t p) {
    idx_t sum = 0;
    while (m) {
        m /= p;
        sum += m;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* C_0 = C_1 = 1 */

    idx_t limit = 2 * n;
    /* Rebuild prime table if needed (monotonic growth) */
    if (limit > cached_prime_limit) {
        if (cached_primes) free(cached_primes);
        build_primes_upto(limit, &cached_primes, &cached_prime_count);
        cached_prime_limit = limit;
    }

    seq_t result = (seq_t)1;
    idx_t n_plus_one = n + (idx_t)1;

    for (size_t i = 0; i < cached_prime_count; ++i) {
        uint64_t p = cached_primes[i];

        /* Exponent of p in C_n */
        idx_t e = legendre_exponent(2 * n, p) - 2 * legendre_exponent(n, p);

        /* Subtract exponent of p in (n+1) */
        idx_t v = 0;
        idx_t temp = n_plus_one;
        while ((temp % p) == 0) {
            ++v;
            temp /= p;
        }
        e -= v;

        if (e > 0) {
            result = seq_mul(result, (seq_t)p);
        }
    }
    return result;
}
