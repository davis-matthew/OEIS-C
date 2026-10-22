/*
 * A354712  Nonprime numbers k such that the number of divisors of k divides
 *          k-1.
 * https://oeis.org/A354712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonprime integer k such that the number of
 *            divisors d(k) divides (k-1)
 * Range:     u8 n <= 37, u16 n <= 6554, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t curr = 1;

static void _grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    seq_t *p = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    cache = p;
    cache_cap = new_cap;
}

static void _generate_up_to(idx_t n) {
    if (n <= cache_len) return;
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;         /* k = 1 is the first term */
        cache_len = 1;
        curr = 1;
    }
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    size_t cnt;

    while (cache_len < (size_t)n) {
        curr++;
        if (is_prime_big((seq_t)curr)) continue;   /* skip primes */

        /* compute divisor count d(curr) */
        factor_u64((uint64_t)curr, primes_arr, exps_arr, &cnt);
        seq_t d = 1;
        for (size_t i = 0; i < cnt; ++i) {
            d *= (seq_t)(exps_arr[i] + 1);
        }

        /* check the property d | (k-1) */
        seq_t rem = ((seq_t)(curr - 1)) % d;
        if (rem == 0) {
            if (cache_len == cache_cap) _grow_cache();
            cache[cache_len++] = (seq_t)curr;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    _generate_up_to(n);
    return cache[n - 1];
}
