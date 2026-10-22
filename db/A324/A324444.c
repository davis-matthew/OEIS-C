/*
 * A324444  a(n) = Product_{i=1..n, j=1..n} (1 + i + j).
 * https://oeis.org/A324444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n, j=1..n} (1 + i + j)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

static inline idx_t sum_floor(idx_t m, idx_t q)
{
    idx_t r = m / q;
    return r * (m + 1) - q * r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* a(0) = 1 */

    /* 2n+1 is the maximum value that appears as (1+i+j) */
    idx_t limit = 2 * n + 1;

    /* Generate all primes <= limit */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t result = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t exp = 0;

        idx_t p_pow = p;
        while (p_pow <= limit) {
            idx_t s1 = sum_floor(limit, p_pow);
            idx_t s2 = sum_floor(n + 1, p_pow);
            /* exponent contributed by this power of the prime */
            exp += s1 - 2 * s2;

            /* advance to next power of p, avoiding overflow */
            if (p_pow > limit / p)
                break;
            p_pow *= p;
        }

        if (exp > 0) {
            seq_t pow_val = seq_pow((seq_t)p, exp);
            result = seq_mul(result, pow_val);
        }
    }

    free(primes);
    return result;
}
