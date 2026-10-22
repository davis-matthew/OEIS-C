/*
 * A305504  Heinz numbers of integer partitions whose distinct parts plus 1
 *          are connected.
 * https://oeis.org/A305504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth Heinz number of a partition whose distinct parts
 *            + 1 are connected
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PRIME_LIMIT 2000000ULL  /* enough for n ≤ 10000 */

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static unsigned *prime_index = NULL;
static bool primes_built = false;

/* Build primes up to PRIME_LIMIT and an index table prime_index[p] = 1‑based index of prime p. */
static void build_primes(void)
{
    if (primes_built) return;
    size_t limit = PRIME_LIMIT;
    uint8_t *sieve = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve) exit(EXIT_FAILURE);
    for (size_t i = 2; i <= limit; ++i) sieve[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }
    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (sieve[i]) prime_cnt++;
    prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);
    prime_index = (unsigned*)calloc(limit + 1, sizeof(unsigned));
    if (!prime_index) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            prime_list[idx] = i;
            prime_index[i] = (unsigned)(idx + 1);   /* 1‑based index */
            ++idx;
        }
    }
    free(sieve);
    primes_built = true;
}

/* Euclidean gcd for unsigned 32 bits. */
static inline unsigned gcd32(unsigned a, unsigned b)
{
    while (b) {
        unsigned t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Test whether integer x is a valid Heinz number (i.e., corresponds to a
   partition whose distinct parts + 1 induce a connected graph). */
static bool candidate_is_good(uint64_t x)
{
    if (x == 1) return true;   /* empty partition */
    uint64_t n = x;
    uint32_t indices[64];
    size_t cnt = 0;

    /* factor n using the precomputed prime list */
    for (size_t pi = 0; pi < prime_cnt && prime_list[pi] * prime_list[pi] <= n; ++pi) {
        uint64_t p = prime_list[pi];
        if (n % p == 0) {
            uint32_t idx = prime_index[p];
            bool present = false;
            for (size_t k = 0; k < cnt; ++k)
                if (indices[k] == idx) { present = true; break; }
            if (!present) indices[cnt++] = idx;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {   /* n is a prime > sqrt(original n) */
        uint32_t idx = prime_index[n];
        bool present = false;
        for (size_t k = 0; k < cnt; ++k)
            if (indices[k] == idx) { present = true; break; }
        if (!present) indices[cnt++] = idx;
    }

    if (cnt <= 1) return true;  /* single vertex is trivially connected */

    uint32_t vertices[64];
    for (size_t i = 0; i < cnt; ++i) vertices[i] = indices[i] + 1;
    uint32_t parent[64];
    for (size_t i = 0; i < cnt; ++i) parent[i] = (uint32_t)i;
    size_t comp = cnt;

    for (size_t a = 0; a < cnt; ++a) {
        for (size_t b = a + 1; b < cnt; ++b) {
            if (gcd32(vertices[a], vertices[b]) > 1) {
                /* union a and b */
                uint32_t ra = a;
                while (parent[ra] != ra) ra = parent[ra];
                uint32_t rb = b;
                while (parent[rb] != rb) rb = parent[rb];
                if (ra != rb) {
                    parent[ra] = rb;
                    --comp;
                }
            }
        }
    }
    return comp == 1;
}

/* Cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* Ensure that cache holds at least target elements. */
static void ensure_cache_size(size_t target)
{
    if (cache_size >= target) return;
    while (cache_cap < target) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }
    while (cache_size < target) {
        uint64_t x = next_candidate++;
        if (candidate_is_good(x)) {
            cache[cache_size++] = (seq_t)x;
        }
    }
}

/* The sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    build_primes();
    ensure_cache_size((size_t)n);
    return cache[n - 1];
}
