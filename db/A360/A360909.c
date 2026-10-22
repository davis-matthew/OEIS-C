/*
 * A360909  Multiplicative with a(p^e) = 3*e + 2.
 * https://oeis.org/A360909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} (3*e + 2) (multiplicative with a(p^e)=3*e+2)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factorize.h"

/* static cache of already computed values (1‑based) */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;

/* static list of primes used for factorisation */
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static idx_t     prime_limit  = 0;   /* maximum n for which prime_list covers all primes ≤ n */

/* helper to grow cache or prime list if needed */
static void _ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t old = cache_cap;
        ensure_capacity(n + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
        /* zero newly allocated part */
        memset(cache + old, 0, (cache_cap - old) * sizeof(seq_t));
    }
    if (n > prime_limit) {
        if (prime_list != NULL)
            free(prime_list);
        /* build primes up to n (inclusive) */
        build_primes_upto(n, &prime_list, &prime_cnt);
        prime_limit = n;
    }
}

/* main entry function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* indices < 1 are not defined */

    _ensure_cache(n);

    if (cache[n] != 0)            /* already computed */
        return cache[n];

    if (n == 1) {                 /* base case */
        cache[1] = 1;
        return 1;
    }

    /* factorise n using the previously built prime list */
    /* we allocate buffers large enough for any n up to 64‑bit */
    static int  exponents[64];
    static uint64_t primes[64];

    size_t pc = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) {
            int e = 0;
            while (n % prime_list[i] == 0) {
                n /= prime_list[i];
                ++e;
            }
            primes[pc]     = prime_list[i];
            exponents[pc]  = e;
            ++pc;
        }
    }
    if (n > 1) {                   /* remaining prime factor */
        primes[pc]     = n;
        exponents[pc]  = 1;
        ++pc;
    }

    seq_t result = 1;
    for (size_t i = 0; i < pc; ++i) {
        result *= (seq_t)(3 * exponents[i] + 2);
    }

    cache[pc] = result;          /* pc is the original n */
    return result;
}
