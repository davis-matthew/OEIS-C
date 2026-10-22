/*
 * A381919  Pentagonal numbers which are products of four distinct primes.
 * https://oeis.org/A381919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th pentagonal number that is a product of four
 *            distinct primes Definition: P(k) = k*(3*k-1)/2 Sequence is
 *            indexed with offset 1
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_PRIME 5000000U /* enough for n up to about 10 000 (see analysis) */

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Simple sieve of Eratosthenes up to MAX_PRIME */
static void init_primes(void) {
    if (primes_initialized) return;
    primes_initialized = true;

    uint32_t limit = MAX_PRIME;
    bool *is_comp = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) exit(1);
    for (uint32_t i = 0; i <= limit; ++i) is_comp[i] = false;

    uint32_t sqr = limit;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                is_comp[(size_t)j] = true;
        }
    }

    /* approximate number of primes to reserve */
    size_t cap = limit / 10;
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list) exit(1);

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_count == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_count++] = i;
        }
    }
    free(is_comp);
}

/* Check whether val is a product of exactly four distinct primes */
static bool is_four_distinct_prime_product(seq_t val) {
    /* numbers of interest are small enough to fit in uint64_t when n≤10000 */
    uint64_t x = (uint64_t)val;
    int cnt = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        uint64_t p64 = p;
        if (p64 * p64 > x) break;
        if (x % p == 0) {
            x /= p;
            ++cnt;
            if (x % p == 0) return false;  /* repeated factor */
            if (cnt > 4) return false;    /* more than four primes */
        }
    }
    if (x > 1) ++cnt;   /* remaining prime factor, if any */
    return cnt == 4;
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;          /* next pentagonal index to test */

    if (!cache) {
        cache = (seq_t *)malloc(1024 * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = 1024;
    }

    init_primes();

    /* Grow cache until it holds at least n terms */
    while (cache_size < n) {
        /* Compute the k-th pentagonal number: k*(3*k-1)/2 */
        seq_t k_seq = (seq_t)next_k;
        seq_t pent = k_seq * (((seq_t)3 * k_seq) - 1) / 2;

        if (is_four_distinct_prime_product(pent)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = pent;
        }
        ++next_k;
    }

    return cache[n - 1];
}
