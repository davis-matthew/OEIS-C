/*
 * A244765  Prime numbers ending in the prime number 19.
 * https://oeis.org/A244765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth prime that ends with 19 (primes of the form
 *            100*k + 19)
 * Range:     u8 n <= 1, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static cache of primes already found */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache */
    static size_t cache_len = 0;   /* number of primes actually stored */

    /* index of the next candidate of the form 100*k + 19 */
    static idx_t next_k = 0;

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = 100 * (seq_t)next_k + 19;   /* candidate 100*k+19 */

        if (isPrimeGeneric(cand)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) { /* allocation failed */
                    mem_free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_len = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_k;   /* move to the next candidate 100*k+19 */
    }

    return cache[n - 1];
}
