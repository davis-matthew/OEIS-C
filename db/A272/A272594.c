/*
 * A272594  Numbers n such that the multiplicative group modulo n is the
 *          direct product of 4 cyclic groups.
 * https://oeis.org/A272594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A272594:  Numbers n such that the multiplicative group modulo n is
   the direct product of 4 cyclic groups.
   Property:  If n = 2^a * Π p_i^{e_i} (p_i odd primes, distinct),
   let t = 0 for a=0 or 1,
           1 for a=2,
           2 for a≥3.
   The number of cyclic factors is (number of distinct odd primes) + t.
   n qualifies iff that sum equals 4. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Simple sieve for primes up to a limit.  The sieve is rebuilt only when
   a larger limit is requested.  The primes are stored in a static array
   for reuse across calls. */
static uint32_t *primes = NULL;
static size_t primes_cnt  = 0;
static size_t primes_lim  = 0;

static void ensure_primes(size_t limit)
{
    if (limit <= primes_lim) return;         /* already sufficient */

    size_t sz = limit + 1;
    uint8_t *is_comp = (uint8_t*)malloc(sz);
    if (!is_comp) abort();
    for (size_t i = 0; i < sz; ++i) is_comp[i] = 0;

    /* Allocate prime array big enough to hold all primes up to 'limit'. */
    uint32_t *new_primes = (uint32_t*)realloc(primes, sz * sizeof(uint32_t));
    if (!new_primes) abort();
    primes = new_primes;

    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_lim = limit;
}

/* Test whether a number satisfies the property. */
static bool qualifies(uint64_t n)
{
    if (n <= 1) return false;

    /* Count exponent of 2. */
    uint32_t exp2 = 0;
    while ((n & 1ULL) == 0) {
        ++exp2;
        n >>= 1;
    }

    /* Count distinct odd primes. */
    uint32_t odd_distinct = 0;
    if (n > 1) {
        uint64_t root = (uint64_t)sqrt((long double)n) + 1;
        ensure_primes((size_t)root);
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > n) break;
            if (n % p == 0) {
                ++odd_distinct;
                while (n % p == 0) n /= p;
            }
        }
        if (n > 1) ++odd_distinct;   /* remaining prime factor */
    }

    /* t depends only on the power of 2. */
    uint32_t t = 0;
    if (exp2 == 2)          t = 1;
    else if (exp2 >= 3)     t = 2;
    /* exp2 0 or 1 => t = 0 */

    return (odd_distinct + t) == 4;
}

/* Cache for computed terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Append a value to the cache, expanding capacity as needed. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 32 : cache_cap << 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; return 0 as sentinel */

    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* Candidate generation starts from the first integer. */
    static uint64_t candidate = 1;  /* will be incremented to 2 on first loop */

    for (;;) {
        ++candidate;               /* move to next integer */
        if (candidate < 2) continue; /* skip 1 */

        if (qualifies(candidate)) {
            cache_append((seq_t)candidate);
            if ((size_t)n <= cache_len) return cache[(size_t)n - 1];
        }
    }
}
