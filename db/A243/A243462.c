/*
 * A243462  Numbers n such that prime(n) and phi(n) have the same decimal
 *          digits.
 * https://oeis.org/A243462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for which prime(n) and φ(n) contain the same set of
 *            decimal digits. The function generates terms in ascending order
 *            and caches them for repeated queries
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   build_primes_upto, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_counts.h"          /* void digit_counts(seq_t x, unsigned char cnt[10]) */
#include "../utils/build_primes_upto.h"     /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) */

static seq_t phi_of(idx_t m, const uint64_t *primes, size_t primes_cnt)
{
    uint64_t result = (uint64_t)m;
    uint64_t temp = result;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result -= result / temp;
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* Static data for caching and precomputation */
    static bool initialized = false;
    static uint64_t *prime_list = NULL;   /* primes up to current_limit */
    static size_t prime_cnt = 0;
    static idx_t current_limit = 1000000; /* initial sieve bound */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static idx_t next_t = 1;              /* next integer to examine */

    if (!initialized) {
        /* Build initial prime table */
        uint64_t *tmp;
        size_t cnt;
        build_primes_upto(current_limit, &tmp, &cnt);
        prime_list = tmp;
        prime_cnt = cnt;

        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Ensure we have enough primes to access prime(next_t) */
        while ((idx_t)(next_t - 1) >= prime_cnt) {
            /* Grow the sieve */
            current_limit *= 2;
            uint64_t *new_primes;
            size_t new_cnt;
            build_primes_upto(current_limit, &new_primes, &new_cnt);
            free(prime_list);
            prime_list = new_primes;
            prime_cnt = new_cnt;
        }

        seq_t phi_val = phi_of(next_t, prime_list, prime_cnt);
        seq_t prime_val = (seq_t)prime_list[(idx_t)(next_t - 1)];

        /* Compare digit sets */
        unsigned char cnt1[10] = {0};
        unsigned char cnt2[10] = {0};
        digit_counts(prime_val, cnt1);
        digit_counts(phi_val, cnt2);

        bool same = true;
        for (int d = 0; d < 10; ++d) {
            if ((cnt1[d] > 0) != (cnt2[d] > 0)) {
                same = false;
                break;
            }
        }

        if (same) {
            /* Store the term in cache */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_t;
        }
        ++next_t;
    }

    return cache[n - 1];
}
