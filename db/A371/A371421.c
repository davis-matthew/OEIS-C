/*
 * A371421  Numbers whose aliquot-like sequence based on the largest aliquot
 *          divisor of the sum of divisors of n (A371418) terminates in a
 *          fixed point.
 * https://oeis.org/A371421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371421: Numbers whose aliquot‑like sequence based on the
   largest proper divisor of σ(n) terminates in a fixed point.
   The function computes a(n) by iterating over successive integers
   and keeping a cache of numbers already known to succeed or fail. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve up to 1 000 000 ---------- */
static uint32_t *prime_list = NULL;
static size_t   prime_count = 0;

static void init_primes(void)
{
    const uint32_t LIMIT = 1000000;
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    prime_list = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    size_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            prime_list[pos++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    prime_count = pos;
    free(is_composite);
}

/* ---------- Sum of divisors σ(n) for 64‑bit n ---------- */
static uint64_t sigma_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = 1;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        if (temp % prime_list[i] == 0) {
            uint64_t p = prime_list[i];
            uint64_t term = 1;
            uint64_t sum = 1;
            while (temp % p == 0) {
                term *= p;
                temp /= p;
                sum += term;
            }
            __uint128_t prod = (__uint128_t)result * sum;
            result = (uint64_t)prod;
        }
    }
    if (temp > 1) {
        __uint128_t prod = (__uint128_t)result * (1 + temp);
        result = (uint64_t)prod;
    }
    return result;
}

/* ---------- Largest proper divisor of m ---------- */
static uint64_t largest_proper_divisor(uint64_t m)
{
    if (m <= 1) return 1;           /* 1 is a fixed point */
    uint64_t temp = m;
    uint64_t spf = 0;               /* smallest prime factor */
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        if (temp % prime_list[i] == 0) {
            spf = prime_list[i];
            break;
        }
    }
    if (spf == 0) spf = temp;        /* m is prime */
    return m / spf;
}

/* ---------- Status cache: 0=unknown, 1=success, 2=failure ---------- */
typedef struct {
    uint64_t *keys;
    uint8_t  *vals;
    size_t   capacity;
    size_t   count;
} StatusCache;

static StatusCache cache;

static void cache_init(void)
{
    cache.capacity = 256;
    cache.count    = 0;
    cache.keys  = (uint64_t *)calloc(cache.capacity, sizeof(uint64_t));
    cache.vals  = (uint8_t  *)calloc(cache.capacity, sizeof(uint8_t));
}

static inline size_t cache_mask(void) { return cache.capacity - 1; }

static uint8_t cache_lookup(uint64_t key)
{
    size_t idx = key & cache_mask();
    while (cache.keys[idx] != 0 && cache.keys[idx] != key)
        idx = (idx + 1) & cache_mask();
    return (cache.keys[idx] == key) ? cache.vals[idx] : 0;
}

static void cache_rehash(void)
{
    size_t new_cap = cache.capacity << 1;
    uint64_t *new_keys = (uint64_t *)calloc(new_cap, sizeof(uint64_t));
    uint8_t  *new_vals = (uint8_t  *)calloc(new_cap, sizeof(uint8_t));
    size_t new_mask = new_cap - 1;

    for (size_t i = 0; i < cache.capacity; ++i) {
        if (cache.keys[i] != 0) {
            uint64_t key = cache.keys[i];
            size_t idx = key & new_mask;
            while (new_keys[idx] != 0)
                idx = (idx + 1) & new_mask;
            new_keys[idx] = key;
            new_vals[idx] = cache.vals[i];
        }
    }
    free(cache.keys);
    free(cache.vals);
    cache.keys = new_keys;
    cache.vals = new_vals;
    cache.capacity = new_cap;
}

static void cache_insert(uint64_t key, uint8_t val)
{
    size_t idx = key & cache_mask();
    while (cache.keys[idx] != 0 && cache.keys[idx] != key)
        idx = (idx + 1) & cache_mask();
    if (cache.keys[idx] == 0) {
        cache.keys[idx] = key;
        cache.vals[idx] = val;
        ++cache.count;
        if (cache.count * 2 >= cache.capacity)
            cache_rehash();
    } else {
        cache.vals[idx] = val;
    }
}

/* ---------- Sequence storage ---------- */
static seq_t *a_seq   = NULL;
static size_t a_len   = 0;
static size_t a_cap   = 16;  /* initial capacity */

/* ---------- Detect if a number eventually reaches a fixed point ---------- */
static bool leads_to_fixed(uint64_t start)
{
    uint64_t *path = NULL;
    size_t path_cap = 0, path_len = 0;
    uint64_t cur = start;

    while (true) {
        uint8_t known = cache_lookup(cur);
        if (known == 1) {            /* succeed */
            for (size_t i = 0; i < path_len; ++i)
                cache_insert(path[i], 1);
            free(path);
            return true;
        }
        if (known == 2) {            /* fail */
            for (size_t i = 0; i < path_len; ++i)
                cache_insert(path[i], 2);
            free(path);
            return false;
        }

        /* add current to path */
        if (path_len == path_cap) {
            path_cap = (path_cap == 0) ? 8 : path_cap << 1;
            path = (uint64_t *)realloc(path, path_cap * sizeof(uint64_t));
        }
        path[path_len++] = cur;

        uint64_t sigma = sigma_uint64(cur);
        uint64_t next  = largest_proper_divisor(sigma);

        if (next == cur) {           /* reached fixed point */
            cache_insert(cur, 1);
            for (size_t i = 0; i < path_len; ++i)
                cache_insert(path[i], 1);
            free(path);
            return true;
        }

        /* cycle detection */
        bool in_path = false;
        for (size_t i = 0; i < path_len; ++i) {
            if (path[i] == next) {
                in_path = true;
                break;
            }
        }
        if (in_path) {                /* cycle of length > 1 */
            for (size_t i = 0; i < path_len; ++i)
                cache_insert(path[i], 2);
            free(path);
            return false;
        }

        cur = next;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static uint64_t max_checked = 0;   /* largest candidate examined so far */

    if (!initialized) {
        init_primes();
        cache_init();
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        initialized = true;
    }

    if (n <= a_len)
        return a_seq[n - 1];

    while (a_len < (size_t)n) {
        uint64_t candidate = max_checked + 1;
        max_checked = candidate;

        if (leads_to_fixed(candidate)) {
            /* grow a_seq if needed */
            if (a_len == a_cap) {
                a_cap <<= 1;
                a_seq = (seq_t *)realloc(a_seq, a_cap * sizeof(seq_t));
            }
            a_seq[a_len++] = (seq_t)candidate;
        }
    }

    return a_seq[n - 1];
}
