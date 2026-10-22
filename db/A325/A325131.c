/*
 * A325131  Heinz numbers of integer partitions where the set of distinct
 *          parts is disjoint from the set of distinct multiplicities.
 * https://oeis.org/A325131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer whose prime factorization satisfies that
 *            the set of prime indices (position in the prime sequence) is
 *            disjoint from the set of exponents
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include minimal headers needed for both C11 and C++17 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum number that will be checked for membership in the sequence.
   3,000,000 is more than enough to obtain the first 10 000 terms. */
#define LIMIT 3000000
/* Maximum number of primes below LIMIT; 80 000 is ample. */
#define MAX_PRIMES 100000

/* Global data needed by the algorithm */
static uint32_t *spf = NULL;                  /* smallest prime factor */
static uint32_t *prime_index = NULL;          /* index of a prime (1‑based) */
static uint32_t primes[MAX_PRIMES];           /* list of primes */
static int prime_count = 0;                   /* number of primes found */
static uint8_t part_used[MAX_PRIMES + 1];     /* marks indices used as part */
static uint8_t exp_used[64];                  /* marks exponents used */
static uint32_t current_candidate = 0;        /* last candidate tested */

/* Cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* One‑time initialization of SPF sieve and prime index array. */
static void init_once(void)
{
    uint32_t i, p;
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    prime_index = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    for (i = 0; i <= LIMIT; ++i) {
        spf[i] = 0;
        prime_index[i] = 0;
    }
    prime_count = 0;
    for (i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            /* i is prime */
            spf[i] = i;
            if (prime_count < MAX_PRIMES) {
                primes[prime_count++] = i;
                prime_index[i] = prime_count;          /* 1‑based */
            }
        }
        for (p = 0; p < (uint32_t)prime_count; ++p) {
            uint32_t pr = primes[p];
            uint64_t val = (uint64_t)i * pr;
            if (val > LIMIT) break;
            spf[(uint32_t)val] = pr;
            if (i % pr == 0) break;
        }
    }
    /* Initialize helper arrays */
    for (i = 0; i <= (uint32_t)prime_count; ++i) part_used[i] = 0;
    for (i = 0; i < 64; ++i) exp_used[i] = 0;
}

/* Check whether value x satisfies the disjointness condition.
   The routine keeps minimal local arrays, thus is cheap. */
static bool satisfies(uint32_t x)
{
    uint32_t t = x;
    /* Since a number < LIMIT has at most 6 distinct prime factors,
       these buffers are more than sufficient. */
    uint32_t part_idx[8];
    uint32_t exp_val[8];
    int part_cnt = 0, exp_cnt = 0;

    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t e = 0;
        do { t /= p; ++e; } while (t % p == 0);

        uint32_t idx = prime_index[p];      /* prime index (1‑based) */
        if (!part_used[idx]) {
            part_used[idx] = 1;
            part_idx[part_cnt++] = idx;
        }
        if (e < 64 && !exp_used[e]) {
            exp_used[e] = 1;
            exp_val[exp_cnt++] = e;
        }
    }

    bool ok = true;
    for (int i = 0; i < part_cnt; ++i) {
        uint32_t idx = part_idx[i];
        if (idx < 64 && exp_used[idx]) {
            ok = false; break;
        }
    }

    /* reset arrays for next call */
    for (int i = 0; i < part_cnt; ++i) part_used[part_idx[i]] = 0;
    for (int i = 0; i < exp_cnt; ++i)  exp_used[exp_val[i]] = 0;

    return ok;
}

/* Ensure the cache holds at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        ++current_candidate;
        if (satisfies(current_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    /* One‑time global initialization */
    static bool initialized = false;
    if (!initialized) {
        init_once();
        initialized = true;
    }
    ensure_cache(n);
    return cache[n - 1];
}
