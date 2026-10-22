/*
 * A369468  a(n) = Product_{k=0..n} ((3*k+1)*(3*k+2))^(n-k).
 * https://oeis.org/A369468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Product_{k=0..n} ((3*k+1)*(3*k+2))^(n-k)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   _prime_index, build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_prime_index.h"

seq_t a(idx_t n)
{
    /* 3*n + 2 is the largest prime factor that can appear */
    uint64_t maxp = 3 * (uint64_t)n + 2;
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(maxp, &primes, &primes_cnt);
    if (!primes) {
        return (seq_t)0;     /* allocation or sieve failure */
    }

    /* exponents of each prime factor */
    uint64_t *prime_exp = (uint64_t *)calloc(primes_cnt, sizeof(uint64_t));
    if (!prime_exp) {
        free(primes);
        return (seq_t)0;     /* allocation failure */
    }

    for (idx_t k = 0; k <= n; ++k) {
        uint64_t g1 = 3 * (uint64_t)k + 1;
        uint64_t g2 = 3 * (uint64_t)k + 2;
        uint64_t g  = g1 * g2;          /* <= 9e8 for n <= 10000 */
        uint64_t exp_factor = (uint64_t)(n - k);   /* exponent multiplier */

        uint64_t rem = g;
        for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= rem; ++pi) {
            uint64_t p   = primes[pi];
            uint64_t cnt = 0;
            while (rem % p == 0) {
                rem /= p;
                ++cnt;
            }
            if (cnt) {
                prime_exp[pi] += cnt * exp_factor;
            }
        }
        if (rem > 1) { /* rem is prime */
            size_t idx = _prime_index(primes, primes_cnt, rem);
            if (idx < primes_cnt) {
                prime_exp[idx] += exp_factor;
            }
        }
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (prime_exp[i]) {
            seq_t powp = seq_pow((seq_t)primes[i], (idx_t)prime_exp[i]);
            result = seq_mul(result, powp);
        }
    }

    free(primes);
    free(prime_exp);
    return result;
}
