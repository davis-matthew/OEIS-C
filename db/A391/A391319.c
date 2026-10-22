/*
 * A391319  Numbers that are neither cubefree nor powerful.
 * https://oeis.org/A391319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer that has at least one prime exponent ≥3 (not
 *            cubefree) and at least one prime exponent =1 (not powerful)
 * Range:     u8 n <= 27, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 1; /* last candidate that was checked */

/* Prime sieve: primes up to 2,000,000 (large enough for n=10000) */
#define PRIME_LIMIT 2000000
static int prime_list[150000];      /* enough to hold primes up to 2e6 */
static size_t prime_cnt = 0;
static bool sieve_done = false;

static void init_primes(void)
{
    if (sieve_done) return;
    sieve_done = true;
    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_cnt++] = i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* Grow cache to at least needed size */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Test if x has a prime exponent ≥3 and a prime exponent =1 */
static bool qualifies(uint64_t x)
{
    bool has_cube = false;
    bool has_single = false;
    uint64_t tmp = x;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        int p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e >= 3) has_cube = true;
            if (e == 1) has_single = true;
            if (has_cube && has_single) return true; /* early exit */
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        /* exponent is exactly 1 */
        if (!has_single) has_single = true;
    }
    return has_cube && has_single;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but just return 0 */

    init_primes();

    /* Extend cache until we have at least n terms */
    while (cache_len < n) {
        ++last_checked;
        if (qualifies(last_checked)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
