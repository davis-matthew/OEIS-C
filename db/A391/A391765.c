/*
 * A391765  a(n) is smallest k such that prime(k) - 1 == 0 (mod n).
 * https://oeis.org/A391765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer k such that prime(k)-1 ≡ 0 (mod n)
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the primes array has capacity at least mincap. */
static void grow_primes(size_t mincap)
{
    if (primes_cap >= mincap) return;
    size_t newcap = primes_cap > 0 ? primes_cap * 2 : 8;
    while (newcap < mincap) newcap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof *tmp);
    primes = tmp;
    primes_cap = newcap;
}

/* Append the next prime to the primes array. */
static void append_next_prime(void)
{
    uint64_t cand;
    if (primes_cnt == 0) cand = 2;
    else {
        cand = primes[primes_cnt - 1] + 1;
        if (cand > 2 && (cand & 1) == 0) ++cand;  /* start at odd */
    }
    while (true) {
        if (cand == 2 || is_prime((idx_t)cand)) {
            grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = cand;
            break;
        }
        cand += (cand == 2 ? 1 : 2);          /* skip even numbers  */
    }
}

/* Ensure the cache array has capacity at least mincap. */
static void grow_cache(size_t mincap)
{
    if (cache_cap >= mincap) return;
    size_t newcap = cache_cap > 0 ? cache_cap * 2 : 8;
    while (newcap < mincap) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof *tmp);
    cache = tmp;
    /* mark new positions as uncomputed (zero). */
    for (size_t i = cache_cap; i < newcap; ++i) cache[i] = 0;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* not used for n < 1, but safe */

    /* Ensure cache capacity */
    if (cache_cap < (size_t)n) grow_cache((size_t)n);
    size_t idx = (size_t)n - 1;
    if (cache[idx] != 0) return cache[idx];

    /* Search for the smallest k with prime(k)-1 ≡ 0 (mod n) */
    for (size_t k = 1; ; ++k) {
        while (primes_cnt < k) append_next_prime();
        uint64_t p = primes[k - 1];
        if ((p - 1ULL) % (uint64_t)n == 0) {
            seq_t val = (seq_t)k;
            cache[idx] = val;
            return val;
        }
    }
}
