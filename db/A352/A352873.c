/*
 * A352873  Heinz numbers of integer partitions with nonnegative crank,
 *          counted by A064428.
 * https://oeis.org/A352873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, for n>1 a(n) = the n-th integer m such that the
 *            integer partition corresponding to m (via the prime-index
 *            product) has nonnegative crank
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- global tables for primes up to a fixed limit ---- */
#define PRIMELIMIT 200000   /* Adjust if a(n) may exceed this. */

static uint32_t *primes = NULL;      /* list of prime numbers */
static size_t     primes_cnt = 0;    /* number of primes */
static uint32_t *prime_index = NULL; /* prime_index[p] = index of prime p (1‑based) */

/* ---- a small cache for already computed terms ---- */
static seq_t *cache = NULL;  /* cache of a(1)...a(k) */
static size_t cache_cap = 0; /* allocated capacity */
static size_t cache_len = 0; /* number of terms stored */

/* last integer examined when building the cache */
static uint64_t last_val = 0;

/* ---- helper to ensure prime tables are built on first use ---- */
static void init_primes(void)
{
    static bool done = false;
    if (done)
        return;
    done = true;

    /* allocate and compute sieve */
    uint32_t *is_prime = (uint32_t *)malloc((PRIMELIMIT + 1) * sizeof(uint32_t));
    if (!is_prime)
        exit(1);
    for (uint32_t i = 0; i <= PRIMELIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIMELIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= PRIMELIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes to allocate table */
    primes_cnt = 0;
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i)
        if (is_prime[i])
            ++primes_cnt;

    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes)
        exit(1);

    prime_index = (uint32_t *)calloc(PRIMELIMIT + 1, sizeof(uint32_t));
    if (!prime_index)
        exit(1);

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i) {
        if (is_prime[i]) {
            primes[idx] = i;
            prime_index[i] = (uint32_t)(idx + 1); /* 1‑based index */
            ++idx;
        }
    }

    free(is_prime);
}

/* ---- helper to grow the cache array when needed ---- */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        exit(1);
    cache = newp;
    cache_cap = new_cap;
}

/* ---- test whether a given m has non‑negative crank ---- */
static bool has_nonnegative_crank(uint64_t m)
{
    if (m == 1)
        return true;           /* empty partition, crank = 0 */

    uint64_t a = 0;            /* exponent of 2 (number of 1's) */
    uint64_t temp = m;
    while ((temp & 1) == 0) {
        ++a;
        temp >>= 1;
    }

    uint64_t b = 0;            /* parts larger than a */
    /* no need for maxk for odd case */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint64_t e = 0;
            uint64_t k = prime_index[p];
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (k > a)
                b += e;
        }
    }

    if (temp > 1) {           /* remaining prime factor */
        uint32_t k = prime_index[(uint32_t)temp];
        if (k > a)
            b += 1;
    }

    /* if a == 0 (no 1's), all odd numbers are valid */
    if (a == 0)
        return true;

    return b >= a;
}

/* ---- the sequence function required by the build ---- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        init_primes();
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
        last_val = 0;
    }

    if (n == 0)
        return 0;  /* offset is 1, this should never happen */

    while (cache_len < (size_t)n) {
        uint64_t cand = last_val + 1;
        while (!has_nonnegative_crank(cand))
            ++cand;
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)cand;
        last_val = cand;
    }

    return cache[n - 1];
}
