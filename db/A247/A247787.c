/*
 * A247787  Sum of divisors of 2*prime(n)-1.
 * https://oeis.org/A247787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(2*prime(n)-1) (sum of divisors of 2*prime(n)-1)
 * Range:     u8 n <= 22, u16 n <= 1928, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   // for factor_u64
#include "../utils/build_primes_upto.h" // for build_primes_upto

seq_t a(idx_t n)
{
    /* Lazily construct prime table once */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;

    if (prime_list == NULL) {
        /* 200000 is comfortably beyond the 10000th prime (104729) */
        build_primes_upto(200000, &prime_list, &prime_count);
    }

    /* Retrieve the n-th prime (1‑based index) */
    uint64_t p = prime_list[n - 1];

    /* Compute N = 2*p - 1 */
    uint64_t N = (uint64_t)p * 2 - 1;

    /* Factor N */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(N, pf, ex, &cnt);

    /* Compute the divisor sum sigma(N) */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pow = 1;
        seq_t sum = 1;   /* 1 + p + p^2 + ... + p^e */
        for (uint64_t j = 0; j < ex[i]; ++j) {
            pow *= pf[i];
            sum += pow;
        }
        sigma *= sum;
    }
    return sigma;
}
