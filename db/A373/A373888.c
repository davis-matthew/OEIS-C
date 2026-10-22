/*
 * A373888  a(n) is the length of the longest arithmetic progression of primes
 *          ending with prime(n).
 * https://oeis.org/A373888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the longest arithmetic progression of
 *            primes ending with Prime(n). Computed by a direct search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Prime flags: 1 = prime, 0 = composite */
static uint8_t *prime_flag = NULL;
static idx_t prime_flag_cap = 0;

/* Extend the sieve to cover all numbers up to 'limit' (inclusive). */
static void ensure_prime_flag(idx_t limit)
{
    if (limit + 1 <= prime_flag_cap) return;

    idx_t old_cap   = prime_flag_cap;
    idx_t new_cap   = limit + 1;
    uint8_t *tmp = (uint8_t *)realloc(prime_flag, new_cap);
    if (!tmp) exit(1);
    prime_flag = tmp;

    /* Initialise new entries as prime */
    for (idx_t i = old_cap; i < new_cap; ++i) prime_flag[i] = 1;
    if (new_cap > 0) prime_flag[0] = 0;
    if (new_cap > 1) prime_flag[1] = 0;

    /* Cross off composites in the new range using existing primes. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime_flag[p]) {
            idx_t start;
            if (old_cap == 0)          /* first time, start at p*p */
                start = p * p;
            else if (old_cap <= p * p) /* old data ended before p*p */
                start = p * p;
            else                       /* start from first multiple >= old_cap */
                start = ((old_cap + p - 1) / p) * p;

            for (idx_t m = start; m <= limit; m += p)
                prime_flag[m] = 0;
        }
    }
    prime_flag_cap = new_cap;
}

/* Memoisation cache for a(n) values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */

    /* Expand cache if needed, keeping previous entries. */
    if (n > cache_cap) {
        idx_t new_cap = n > (cache_cap << 1) ? n : (cache_cap << 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0; /* uncomputed */
        cache_cap = new_cap;
    }

    if (cache[n - 1] != 0)           /* already computed */
        return cache[n - 1];

    /* Get the nth prime and ensure the sieve covers it. */
    uint32_t p = get_prime(n);
    ensure_prime_flag(p);

    uint64_t best = 1;

    /* Try all possible common differences using earlier primes. */
    for (idx_t j = 1; j < n; ++j) {
        uint32_t prev = get_prime(n - j);
        uint64_t d    = (uint64_t)p - (uint64_t)prev;
        uint64_t cur  = (uint64_t)p;
        uint64_t cnt  = 1;

        while (cur > d && cur > 0) {
            uint64_t next = cur - d;
            if (!prime_flag[(idx_t)next]) break;
            cur = next;
            ++cnt;
        }
        if (cnt > best) best = cnt;
        if (best == n) break;              /* cannot be longer than n */
    }

    cache[n - 1] = (seq_t)best;
    return cache[n - 1];
}
