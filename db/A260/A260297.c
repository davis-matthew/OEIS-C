/*
 * A260297  a(n) = prime(n) - (hyperfactorial(prime(n)-1) mod prime(n)).
 * https://oeis.org/A260297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) - hyperfactorial(prime(n)-1) mod prime(n)
 * Range:     u8 n <= 55, u16 n <= 6545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple sieve cache of the first primes and modular exponentiation.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* generic modular exponentiation using 128‑bit intermediate products */
static uint64_t modpow_u64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t cur = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (unsigned __int128)result * cur % mod;
        cur = (unsigned __int128)cur * cur % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)         /* offset 1; undefined for n==0 – return 0 as guard */ 
        return 0;

    /* static cache of primes */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cnt = 0;
    static size_t cache_limit = 0;

    /* enlarge cache if necessary (for n <= 10000, n*20 is a safe bound) */
    if (n > prime_cnt)
    {
        size_t new_limit = (size_t)n * 20 + 100;   /* crude upper bound   */
        if (new_limit < 100) new_limit = 100;
        if (new_limit > cache_limit)
        {
            free(prime_cache);
            prime_cache = NULL;
            build_primes_upto((idx_t)new_limit, &prime_cache, &prime_cnt);
            cache_limit = new_limit;
        }
    }

    uint64_t p = prime_cache[n - 1];           /* prime(n)              */
    uint64_t prod = 1;                          /* hyperfactorial mod p */

    for (idx_t k = 1; k < p; ++k)
    {
        uint64_t kpow = modpow_u64(k, k, p);   /* k^k mod p             */
        prod = (unsigned __int128)prod * kpow % p;
    }

    uint64_t result = p - prod;                /* a(n) = prime - modprod */
    return (seq_t)result;
}
