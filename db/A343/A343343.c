/*
 * A343343  Numbers with either no prime index dividing, or no prime index
 *          divisible by all the other prime indices.
 * https://oeis.org/A343343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Defined by searching numbers where the smallest prime index does not
 * divide all other prime indices, OR the largest prime index does not
 * divide all other prime indices.  The primes are indexed starting at 1
 * (prime 2 → index 1, prime 3 → index 2, …). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime sieve up to a fixed limit ---------- */
#define SIEVE_LIMIT 2000000

static uint32_t *prime_list = NULL;      /* primes in order, 1‑based indices */
static size_t   prime_count = 0;         /* number of primes stored */
static uint32_t *prime_to_idx = NULL;    /* prime_to_idx[p] = index of prime p; 0 if not prime */
static bool     sieve_done = false;

static void init_sieve(void)
{
    if (sieve_done)
        return;

    sieve_done = true;

    /* Allocate space for prime list (reserve 150k primes) and index map */
    prime_list = (uint32_t *)malloc(150000 * sizeof(uint32_t));
    prime_count = 0;
    prime_to_idx = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));

    /* Simple sieve of Eratosthenes */
    bool *is_composite = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= (size_t)SIEVE_LIMIT; ++i)
        is_composite[i] = false;

    for (uint32_t i = 2; i <= (uint32_t)SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            /* add prime */
            if (prime_count >= 150000) {    /* grow if needed */
                prime_list = (uint32_t *)realloc(prime_list, (prime_count + 100000) * sizeof(uint32_t));
            }
            prime_list[prime_count++] = i;
            prime_to_idx[i] = (uint32_t)(prime_count); /* indices start at 1 */

            if ((uint64_t)i * i <= (uint64_t)SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= (uint32_t)SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ---------- Test whether a number satisfies the property ---------- */
static bool is_good(unsigned long long x)
{
    if (x == 1)
        return true;        /* 1 has an empty multiset */

    /* Build the multiset of prime indices (with multiplicity) */
    uint32_t indices[64];   /* enough for numbers that fit in 64‑bit */
    size_t idx_cnt = 0;
    unsigned long long n = x;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((unsigned long long)p * p > n)
            break;
        if (n % p == 0) {
            uint32_t ind = prime_to_idx[p];
            while (n % p == 0) {
                if (idx_cnt < sizeof(indices) / sizeof(indices[0]))
                    indices[idx_cnt++] = ind;
                n /= p;
            }
        }
    }
    if (n > 1) { /* remaining prime factor */
        if (n <= (unsigned long long)SIEVE_LIMIT) {
            uint32_t ind = prime_to_idx[(uint32_t)n];
            if (idx_cnt < sizeof(indices) / sizeof(indices[0]))
                indices[idx_cnt++] = ind;
        } else {
            /* prime exceeds sieve limit (unlikely for our bounds). */
            /* Treat its index as > SIEVE_LIMIT, causing it to not divide
             * any other smaller index, which suffices for the test.  */
            if (idx_cnt < sizeof(indices) / sizeof(indices[0]))
                indices[idx_cnt++] = (uint32_t)SIEVE_LIMIT + 1;
        }
    }

    /* Find smallest and largest indices */
    uint32_t s = UINT32_MAX, g = 0;
    for (size_t i = 0; i < idx_cnt; ++i) {
        if (indices[i] < s) s = indices[i];
        if (indices[i] > g) g = indices[i];
    }

    /* Condition 1: smallest index divides all others */
    bool cond1 = true;
    for (size_t i = 0; i < idx_cnt; ++i) {
        if (indices[i] != s && indices[i] % s != 0) {
            cond1 = false;
            break;
        }
    }

    /* Condition 2: largest index divisible by all others */
    bool cond2 = true;
    for (size_t i = 0; i < idx_cnt; ++i) {
        if (indices[i] != g && g % indices[i] != 0) {
            cond2 = false;
            break;
        }
    }

    /* Good if at least one condition fails */
    return !(cond1 && cond2);
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* Prime tables must be built once */
    init_sieve();

    /* Cache of found sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static unsigned long long current = 1; /* number to test next */

    /* Grow cache until it contains a(n) */
    while (cache_len < (size_t)n) {
        if (is_good(current)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)current;
        }
        ++current;
    }
    return cache[n - 1];
}
