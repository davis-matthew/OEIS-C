/*
 * A342526  Heinz numbers of integer partitions with weakly decreasing first
 *          quotients.
 * https://oeis.org/A342526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose partition (indices of
 *            prime factors) has weakly decreasing first quotients. a(1) = 1,
 *            a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5, … as listed in OEIS
 *            entry
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----------------- Prime utilities ----------------- */
#define PRIME_LIMIT 200000  /* enough for n up to about 10000 */

static uint32_t *sieve_primes = NULL;
static size_t sieve_prime_count = 0;
static int *prime_index_map = NULL;    /* index = i+1 */

static void build_sieve(void)
{
    if (sieve_primes != NULL) return;        /* already built */

    /* simple sieve */
    char *is_prime = (char*)calloc(PRIME_LIMIT + 1, 1);
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;

    size_t count = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) {
            if (count == 0) sieve_primes = (uint32_t*)malloc(PRIME_LIMIT * sizeof(uint32_t));
            sieve_primes[count++] = (uint32_t)i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i) is_prime[j] = 0;
        }
    sieve_prime_count = count;

    /* build index map */
    prime_index_map = (int*)calloc(PRIME_LIMIT + 1, sizeof(int)); /* 0 means not prime */
    for (size_t i = 0; i < sieve_prime_count; ++i)
        prime_index_map[sieve_primes[i]] = (int)(i + 1);
}

/* binary search for prime index if not in map */
static int prime_index(uint32_t p)
{
    if (p <= PRIME_LIMIT) {
        int idx = prime_index_map[p];
        if (idx) return idx;
    }
    /* fallback: linear search (should not happen for our limits) */
    for (size_t i = 0; i < sieve_prime_count; ++i)
        if (sieve_primes[i] == p) return (int)(i + 1);
    return -1; /* not found */
}

/* ----------------- Check integer ----------------- */
static int qualifies(uint64_t m)
{
    if (m == 1) return 1;        /* 1 has empty partition */

    build_sieve();               /* ensure primes ready */

    uint64_t temp = m;
    /* array of part indices, trivial size */
    int parts[32];
    size_t pc = 0;

    for (size_t i = 0; i < sieve_prime_count; ++i) {
        uint32_t p = sieve_primes[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            int idx = (int)(i + 1);
            int cnt = 0;
            while (temp % p == 0) {
                ++cnt;
                temp /= p;
            }
            for (int k = 0; k < cnt; ++k) parts[pc++] = idx;
        }
    }
    if (temp > 1) {               /* remaining prime factor > sqrt(m) */
        int idx = prime_index((uint32_t)temp);
        if (idx <= 0) return 0;    /* should not happen */
        parts[pc++] = idx;
    }

    /* sort descending */
    if (pc > 1) {
        /* simple insertion sort */
        for (size_t i = 1; i < pc; ++i) {
            int key = parts[i];
            int j = (int)i - 1;
            while (j >= 0 && parts[j] < key) {
                parts[j + 1] = parts[j];
                --j;
            }
            parts[j + 1] = key;
        }
    }

    /* check weakly decreasing first quotients:
       For triple (p[i], p[i+1], p[i+2]), require p[i+1]^2 >= p[i]*p[i+2] */
    for (size_t i = 0; i + 2 < pc; ++i) {
        uint64_t a = (uint64_t)parts[i];
        uint64_t b = (uint64_t)parts[i + 1];
        uint64_t c = (uint64_t)parts[i + 2];
        if (b * b < a * c) return 0;
    }
    return 1;
}

/* ----------------- Sequence cache ----------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ----------------- Public function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard */
    /* 1-indexed */
    if (cache_len < n) {
        /* initialize first term */
        if (cache_len == 0) {
            ensure_cache_cap(1);
            cache[0] = (seq_t)1;
            cache_len = 1;
            next_candidate = 2;
        }
        while (cache_len < (size_t)n) {
            if (qualifies((uint64_t)next_candidate)) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
