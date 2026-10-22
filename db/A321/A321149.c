/*
 * A321149  a(1) = 102735, a(n) = prime(n-1)*a(n-1) but products that are not
 *          in A010784 are first reduced as in A320486 (see comments);
 *          continue until zero is reached.
 * https://oeis.org/A321149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 93, u64 n <= 362, u128 n <= 362,
 *            big any n
 * Helpers:   digit_len, get_digits, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A321149: a(1)=102735;
 * For n>1: a(n) = reduce( prime(n-1) * a(n-1) );
 * The reduce function removes every digit that appears more than once
 * (erasing all its occurrences). Leading zeros are ignored; if all digits
 * disappear the result is 0.  The sequence therefore only contains
 * numbers with unique digits (A010784).  Prime(1)=2. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/digit_len.h"
#include "../utils/seq_mul.h"

/* Helper: reduce a number by removing duplicate digits.
   The digits are returned by get_digits MSB->LSB, but we
   reconstruct them in the correct order regardless. */
static seq_t reduce_digits(seq_t x)
{
    size_t len = digit_len(x);
    if (len == 0)
        return (seq_t)0;

    /* Allocate buffer for the digits. */
    uint8_t *buf = (uint8_t *)malloc(len);
    if (!buf)
        return (seq_t)0;  /* Out‑of‑memory fallback */

    size_t real_len;
    get_digits(x, buf, &real_len);

    unsigned char freq[10] = {0};
    for (size_t i = 0; i < real_len; ++i)
        ++freq[buf[i]];

    seq_t res = 0;
    /* rebuild digits preserving original order */
    for (int i = (int)real_len - 1; i >= 0; --i)
        if (freq[buf[i]] == 1)
            res = res * 10 + (seq_t)buf[i];

    free(buf);
    return res;
}

/* Simple sieve to generate first ~20000 primes (enough for n≤10000). */
static uint64_t *prime_tbl = NULL;
static size_t prime_cap = 0;

static void init_primes(void)
{
    if (prime_tbl)
        return;

    const size_t LIMIT = 200000; /* contains >18000 primes */
    bool *comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!comp)
        return; /* fallback: array is NULL */

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                comp[j] = true;
        }
    }

    /* Count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!comp[i])
            ++prime_cap;

    prime_tbl = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!comp[i])
            prime_tbl[idx++] = i;
    }

    free(comp);
}

/* Memoized cache for sequence values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static void ensure_cache_size(idx_t n)
{
    if (n <= cache_cap)
        return;

    size_t new_cap = n * 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* abort on failure */

    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return term a(n) of A321149. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    init_primes();

    /* Base. */
    if (cache_size == 0) {
        ensure_cache_size(1);
        cache[0] = 102735;
        cache_size = 1;
    }

    /* Compute missing terms if required. */
    if ((size_t)n > cache_size) {
        ensure_cache_size(n);
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            /* primes are 1‑based: prime(1)=2. For a(k) use prime(k-1). */
            uint64_t p = prime_tbl[(size_t)(k - 2)];
            seq_t prod = seq_mul(cache[(size_t)(k - 2)], (seq_t)p);
            cache[(size_t)(k - 1)] = reduce_digits(prod);
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)(n - 1)];
}
