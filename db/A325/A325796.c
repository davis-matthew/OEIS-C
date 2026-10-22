/*
 * A325796  Numbers with at least as many divisors as the sum of their prime
 *          indices.
 * https://oeis.org/A325796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m such that * d(m) ≥ Σ_{p^e‖m}
 *            e * idx(p), * where idx(p) is the 1‑based index of prime p in
 *            the list of all primes * (2 is index 1)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*---------- static state ----------*/
static uint64_t *prime_list = NULL;      /* ascending primes                 */
static size_t   prime_cnt   = 0;         /* number of primes stored          */
static size_t   sieve_limit = 0;         /* highest number sieved            */

static seq_t *cache      = NULL;         /* cached terms of the sequence     */
static size_t cache_len  = 0;            /* number of stored terms           */
static size_t cache_cap  = 0;            /* allocated capacity of cache      */

static uint64_t candidate = 1;          /* next integer to test             */

/*---------- helper functions ----------*/

/* Sieve of Eratosthenes: ensure prime_list contains all primes ≤ limit */
static void ensure_primes(size_t limit)
{
    if (limit <= sieve_limit) return;

    bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_composite) return;           /* allocation failure: abort silently */
    is_composite[0] = is_composite[1] = true;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }
    size_t new_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++new_cnt;

    uint64_t *new_list = (uint64_t*)malloc(new_cnt * sizeof(uint64_t));
    if (!new_list) {                      /* allocation failure: abort silently */
        free(is_composite);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) new_list[idx++] = i;

    free(is_composite);
    free(prime_list);
    prime_list = new_list;
    prime_cnt   = new_cnt;
    sieve_limit = limit;
}

/* Binary search for the 1‑based index of prime p in prime_list */
static inline size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;   /* 1‑based index */
}

/* Test whether integer n satisfies the divisibility inequality */
static bool satisfies_uint64(uint64_t n)
{
    uint64_t tmp = n;
    uint64_t d   = 1;       /* divisor count */
    uint64_t sumIdx = 0;    /* weighted sum of prime indices */

    if (n > sieve_limit) ensure_primes((size_t)n);

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            unsigned int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            d       *= (cnt + 1);
            sumIdx += (uint64_t)cnt * (uint64_t)prime_index(p);
        }
    }
    if (tmp > 1) {           /* remaining prime factor */
        d       *= 2;
        sumIdx += prime_index(tmp);
    }
    return d >= sumIdx;
}

/* Ensure cache has capacity for at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;        /* allocation failure: abort silently */
    cache = tmp;
    cache_cap = new_cap;
}

/* Populate the cache until it contains at least 'target' terms */
static void expand_to(idx_t target)
{
    while (cache_len < (size_t)target) {
        if (satisfies_uint64(candidate)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/*---------- public function ----------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe-return */

    /* Lazily initialize on first call */
    if (!cache) {
        cache_len = 0;
        cache_cap = 0;
        candidate = 1;
        sieve_limit = 0;
        prime_list = NULL;
        prime_cnt  = 0;
    }

    if (cache_len < (size_t)n)
        expand_to(n);

    return cache[n - 1];
}
