/*
 * A295450  a(n) = (14*n)!*(6*n)!*(4*n)!/((12*n)!*(7*n)!*(3*n)!*(2*n)!).
 * https://oeis.org/A295450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(14*n,2*n) * binomial(4*n,n) / binomial(7*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/build_primes_upto.h"

/* Legendre's formula: exponent of prime p in n! */
static inline idx_t legendre(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n) {
        n /= p;
        res += n;
    }
    return res;
}

/* Cache of primes up to the largest limit requested so far */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t limit = 14 * n;
    if (limit > prime_cache_limit) {
        /* allocate new prime list */
        free(prime_cache);
        prime_cache = NULL;
        build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
        prime_cache_limit = limit;
    }

    seq_t result = 1;

    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        idx_t p = prime_cache[i];
        /* exponent of p in the formula */
        long long exp = 0;
        exp += legendre(14 * n, p) - legendre(12 * n, p) - legendre(2 * n, p);
        exp += legendre(4 * n, p)  - legendre(3 * n, p) - legendre(n, p);
        exp -= legendre(7 * n, p)  - legendre(6 * n, p) - legendre(n, p);

        if (exp > 0) {
            seq_t term = big_pow((seq_t)p, (seq_t)exp);
            result = seq_mul(result, term);
        }
    }

    return result;
}
