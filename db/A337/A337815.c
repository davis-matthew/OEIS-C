/*
 * A337815  Primes p such that p+q = r*(s+1) where q is the next prime after
 *          p, and r and s are primes.
 * https://oeis.org/A337815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p + next_prime(p) = r*(s+1) with r and s
 *            primes
 * Range:     u8 n <= 36, u16 n <= 3026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

#define PRIME_LIMIT 2000000U

static void
build_primes(uint32_t **primes_out, size_t *cnt_out, uint32_t limit)
{
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sieve) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t count = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            ++count;
            if ((uint64_t)i * i <= limit)
                for (uint32_t j = i * i; j <= limit; j += i)
                    sieve[j] = true;
        }
    }

    uint32_t *pr = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!pr) {
        free(sieve);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve[i]) pr[idx++] = i;

    free(sieve);
    *primes_out = pr;
    *cnt_out = count;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_idx = 0;          /* current position in primes list */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* first call: prime table and cache */
    if (!initialized) {
        build_primes(&primes, &prime_cnt, PRIME_LIMIT);
        if (!primes || prime_cnt < 2) return 0;
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        initialized = true;
    }

    /* fill cache until we have at least n primes satisfying the condition */
    while (cache_len < (size_t)n && (prime_idx + 1) < prime_cnt) {
        uint32_t p = primes[prime_idx];
        uint32_t q = primes[prime_idx + 1];
        uint64_t N = (uint64_t)p + (uint64_t)q;

        bool good = false;
        /* iterate over potential r */
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t r = primes[j];
            if ((uint64_t)r * (uint64_t)r > N) break;
            if (N % r != 0) continue;

            uint64_t s_plus1 = N / r;
            if (s_plus1 <= 1) continue;
            uint64_t s = s_plus1 - 1;
            if (s < 2) continue;

            if ((uint64_t)r < s && is_prime((idx_t)s)) {
                good = true;
                break;
            }
        }

        if (good) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    free(primes);
                    return 0;
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)p;
        }
        ++prime_idx;
    }

    if (n && n <= cache_len)
        return cache[n - 1];
    return 0;
}
