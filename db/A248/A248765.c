/*
 * A248765  Greatest k such that k^4 divides n!
 * https://oeis.org/A248765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 31, u32 n <= 48, u64 n <= 77, u128 n <= 129,
 *            big any n
 * Helpers:   build_primes, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the greatest k such that k^4 divides n! */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Gather all primes ≤ n */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(n, &primes, &prime_cnt);

    seq_t result = 1;          /* initialise the product */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = primes[i];
        /* LeGendre formula for exponent of p in n! */
        idx_t e = 0;
        idx_t power = p;
        while (power <= n)
        {
            e += n / power;
            if (power > n / p) break;   /* avoid overflow */
            power *= p;
        }

        idx_t r = e / 4;   /* floor division gives exponent in k */
        if (r > 0)
        {
            seq_t p_pow = pow_uint((uint64_t)p, r);
            result = seq_mul(result, p_pow);
        }
    }

    free(primes);   /* clean up the primes array */

    return result;
}
