/*
 * A358552  Node-height of the rooted tree with Matula-Goebel number n. Number
 *          of nodes in the longest path from root to leaf.
 * https://oeis.org/A358552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9998, u16 n <= 9998, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Node‑height of a rooted tree with Matula‑Goebel number n.
   Formula: a(1)=1; for n>1, a(n)=1+max_{p|n} a(index(p)),
   where index(p) is the 1‑based ordinal of prime p. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static data for primes and memoisation */
static uint64_t *primes = NULL;      /* list of primes */
static size_t   primes_cnt = 0;      /* number of primes */
static idx_t    primes_limit = 0;    /* current sieve limit */

static seq_t *height_cache = NULL;   /* memoised heights */
static idx_t  cache_cap = 0;         /* cache capacity */

/* create a list of all primes up to 'limit' (inclusive) */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    size_t sz = (size_t)limit + 1;
    unsigned char *sieve = (unsigned char*)calloc(sz, 1);
    if (!sieve)
        return;                       /* allocation failure – leave empty */

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = 1;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (!sieve[i])
            ++count;

    uint64_t *new_primes = (uint64_t*)malloc(count * sizeof(uint64_t));
    if (new_primes) {
        size_t idx = 0;
        for (size_t i = 2; i <= (size_t)limit; ++i)
            if (!sieve[i])
                new_primes[idx++] = (uint64_t)i;
    }

    free(sieve);
    free(primes);
    primes = new_primes;
    primes_cnt = count;
    primes_limit = limit;
}

/* 1‑based index of a prime number 'p' (p ≥ 2) */
static idx_t prime_index_of(uint64_t p)
{
    if (p < 2)
        return 0;                      /* not a prime */

    ensure_primes_upto(p);
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t val = primes[mid];
        if (val == p)
            return (idx_t)(mid + 1);   /* 1‑based */
        if (val < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;                          /* should never happen for a true prime */
}

/* smallest prime factor of n (  n ≥ 2 ) */
static idx_t smallest_prime_factor(idx_t n)
{
    if (n < 2)
        return n;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (n % i == 0)
            return i;
    }
    return n;                          /* n is prime */
}

/* ensure memoisation cache can hold indices up to 'needed' */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_cap)
        return;

    idx_t new_cap = needed + 1;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;

    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = height_cache[i];
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    free(height_cache);
    height_cache = new_cache;
    cache_cap = new_cap;
}

/* recursive calculation of node‑height with memoisation */
static seq_t node_height(idx_t n)
{
    if (n == 1)
        return 1;                     /* leaf */

    ensure_cache(n);
    if (height_cache[n] != 0)
        return height_cache[n];

    seq_t max_child = 0;
    idx_t temp = n;
    while (temp > 1) {
        uint64_t spf = smallest_prime_factor(temp);
        idx_t child = prime_index_of(spf);
        seq_t h = node_height(child);
        if (h > max_child)
            max_child = h;
        while (temp % spf == 0)
            temp /= spf;
    }

    seq_t res = max_child + 1;
    height_cache[n] = res;
    return res;
}

/* required external function */
seq_t a(idx_t n)
{
    return node_height(n);
}
