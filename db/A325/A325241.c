/*
 * A325241  Numbers > 1 whose maximum prime exponent is one greater than their
 *          minimum.
 * https://oeis.org/A325241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A325241.  Numbers > 1 whose maximum prime exponent is one greater than their minimum. */
/* Implementation: generate integers in increasing order and test the exponent-condition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static state to store the sequence and sweep information */
static seq_t *seq_cache = NULL;          /* Cached sequence values */
static size_t cache_len = 0;             /* Number of cached values */
static size_t cache_cap = 0;             /* Allocated capacity of seq_cache */
static uint64_t scan_start = 2;          /* Next integer to test */

static uint32_t *prime_list = NULL;      /* List of primes for trial division */
static size_t prime_cnt = 0;             /* Number of primes stored */
static size_t prime_limit = 0;           /* Current maximum prime value stored */

/* Ensure that prime_list contains all primes up to 'limit'.  Sieve of Eratosthenes. */
static void ensure_primes_upto(size_t limit) {
    if (limit <= prime_limit)
        return;
    /* Allocate temporary array for sieve */
    size_t n = limit;
    bool *is_comp = (bool *)calloc(n + 1, sizeof(bool));
    if (!is_comp) exit(1);
    for (size_t i = 2; i * i <= n; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_comp[j] = true;
        }
    }
    /* Count primes to reallocate prime_list */
    size_t new_cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (!is_comp[i]) ++new_cnt;
    /* Reallocate prime_list */
    uint32_t *new_list = (uint32_t *)realloc(prime_list, new_cnt * sizeof(uint32_t));
    if (!new_list) exit(1);
    prime_list = new_list;
    prime_cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (!is_comp[i]) prime_list[prime_cnt++] = (uint32_t)i;
    free(is_comp);
    prime_limit = limit;
}

/* Grow cache to accommodate at least 'needed' elements */
static void grow_cache(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Test whether integer x satisfies the condition (max exp = min exp + 1 and at least two distinct primes) */
static bool is_valid_candidate(uint64_t x) {
    if (x <= 1)
        return false;

    uint64_t n = x;
    int min_exp = -1, max_exp = -1;
    int distinct = 0;

    /* Ensure the prime list covers sqrt(n) */
    size_t sqrt_n = (size_t)(sqrt((double)n) + 0.5);
    if (sqrt_n > prime_limit)
        ensure_primes_upto(sqrt_n);

    for (size_t idx = 0; idx < prime_cnt && (uint64_t)prime_list[idx] * (uint64_t)prime_list[idx] <= n; ++idx) {
        uint32_t p = prime_list[idx];
        if (n % p == 0) {
            ++distinct;
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (min_exp == -1 || exp < min_exp) min_exp = exp;
            if (max_exp == -1 || exp > max_exp) max_exp = exp;
        }
    }
    if (n > 1) {            /* Remaining prime factor */
        ++distinct;
        int exp = 1;
        if (min_exp == -1 || exp < min_exp) min_exp = exp;
        if (max_exp == -1 || exp > max_exp) max_exp = exp;
    }

    if (distinct < 2)
        return false;
    if (max_exp - min_exp != 1)
        return false;
    return true;
}

/* Generate sequence values until we have at least 'n' terms */
static void ensure_terms(idx_t n) {
    if (n == 0)
        return;
    while (cache_len < (size_t)n) {
        uint64_t cand = scan_start++;
        if (is_valid_candidate(cand)) {
            grow_cache(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)cand;
        } else {
            /* For speed, we can skip to next candidate that could satisfy the condition,
               but simple increment is adequate for n <= 10000. */
        }
    }
}

/* Function required by the exercise */
seq_t a(idx_t n) {
    if (n == 0)          /* Undefined for offset < 1, but return 0 */
        return 0;
    ensure_terms(n);
    return seq_cache[n - 1];
}
