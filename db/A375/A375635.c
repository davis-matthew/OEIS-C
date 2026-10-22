/*
 * A375635  The number of infinitary divisors of n!.
 * https://oeis.org/A375635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{∑_{p≤n} popcount(⌊n/p⌋+⌊n/p²⌋+…)}
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 63, u64 n <= 176,
 *            u128 n <= 385, big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts the infinitary divisors of n!                    */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Special cases: n=0 or n=1 */
    if (n < 2)
        return pow2(0);                 /* 2⁰ = 1 */

    /* Get list of primes up to n */
    size_t prime_cnt = 0;
    uint64_t *primes   = NULL;
    build_primes_upto(n, &primes, &prime_cnt);
    if (!primes)
        return pow2(0);                 /* allocator failure -- fallback */

    idx_t total_pop = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p   = primes[i];
        idx_t exp = 0;
        idx_t t   = p;

        /* Legendre's formula for exponent of p in n! */
        while (t <= n) {
            exp += n / t;
            /* Stop before t*p would overflow idx_t */
            if (t > n / p)
                break;
            t *= p;
        }

        /* Add popcount of exp to total */
        total_pop += __builtin_popcountll((unsigned long long)exp);
    }

    free(primes);
    return pow2(total_pop);             /* 2^{total_pop} */
}
