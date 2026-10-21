/*
 * A268062  a(n) = Sum_{k=1..n} prime(k+1)^prime(k).
 * https://oeis.org/A268062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k+1)^prime(k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"
#include "../utils/_prime_limit_for_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */

    static uint32_t *primes = NULL;
    static size_t primes_count = 0;

    /* Need primes[0..n] (prime(1) .. prime(n+1)) */
    const idx_t required = n + 1;
    if (primes_count < (size_t)required)
    {
        idx_t limit = _prime_limit_for_idx(required);
        size_t got = 0;
        uint64_t *tmp = NULL;

        /* Rebuild until we have enough primes */
        while (1)
        {
            build_primes_upto(limit, &tmp, &got);
            if (got >= (size_t)required) break;
            free(tmp);
            limit *= 2;
        }

        /* Convert uint64_t primes to uint32_t for faster access */
        uint32_t *newpr = (uint32_t *)malloc(got * sizeof(uint32_t));
        if (!newpr)
        {
            free(tmp);
            return 0; /* allocation failed – return zero as a sentinel */
        }
        for (size_t i = 0; i < got; ++i) newpr[i] = (uint32_t)tmp[i];
        free(tmp);

        /* Replace previous cache if any */
        free(primes);
        primes = newpr;
        primes_count = got;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t base = (seq_t)primes[k];     /* prime(k+1) */
        seq_t exp  = (seq_t)primes[k - 1]; /* prime(k)   */
        sum += big_pow(base, exp);
    }
    return sum;
}
