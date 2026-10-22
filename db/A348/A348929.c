/*
 * A348929  a(n) = gcd(n, A003959(n)), where A003959 is multiplicative with
 *          a(p^e) = (p+1)^e.
 * https://oeis.org/A348929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = gcd( n , Π (p+1)^e ) where n = Π p^e (A003959)
 * Range:     u8 n <= 47, u16 n <= 9303, u32 n <= 9303, u64 n <= 9303,
 *            u128 n <= 9303, big any n
 * Helpers:   big_pow, ensure_cache, gcd, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/ensure_cache.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term */
    if (n == 0)
        return 0;        /* undefined for n==0 */

    /* static cache: cache[i] holds a(i) once computed; 0 indicates "not yet" */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache == NULL || n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    if (cache[n] != 0)
        return cache[n];

    /* Compute A003959(n) = Π (p+1)^e over prime powers of n */
    seq_t prod = 1;
    uint64_t temp = (uint64_t)n;

    while (temp > 1) {
        uint64_t p = smallest_prime_factor(temp);
        uint64_t e = 0;
        do {
            temp /= p;
            ++e;
        } while (temp % p == 0);

        seq_t base = (seq_t)(p + 1);
        seq_t pk   = big_pow(base, (seq_t)e);
        prod *= pk;
    }

    seq_t res = gcd(prod, (seq_t)n);

    cache[n] = res;
    return res;
}
