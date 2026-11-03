/*
 * A300226  Filter sequence combining A046523(n) and A052126(n), the prime
 *          signature of n and n/(largest prime dividing n).
 * https://oeis.org/A300226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the restricted growth sequence of the Cantor pairing
 *            P(A046523(n), A052126(n)) for n ≥ 1, where A046523(n) = number
 *            of distinct prime factors of n, A052126(n) = n / (largest prime
 *            dividing n)
 * Range:     u8 n <= 2020, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9846 (2026-11-03)
 * Helpers:   mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>
#include "../utils/omega.h"
#include "../utils/mem.h"

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

/* Cantor pairing for non‑negative seq_t values. */
static inline seq_t cantor(seq_t a, seq_t b)
{
    seq_t s = a + b;
    seq_t t = (s * (s + 1)) / 2 + b;
    return t;
}

/* Memoised implementation of A300226. */
seq_t a(idx_t n)
{
    /* Undefined for n = 0 – return 0 (indexing starts at 1). */
    if (n == 0)
        return 0;

    /* Static cache of computed a(i). */
    static seq_t *cache       = NULL;      /* a(1)...a(current) */
    static idx_t  cache_len   = 0;         /* number of entries stored */
    static idx_t  cache_cap   = 0;         /* allocated size of cache */

    /* Mapping from Cantor value to the index of its first occurrence. */
    static seq_t *pair_keys   = NULL;      /* Cantor values */
    static idx_t *pair_ids    = NULL;      /* corresponding first indices */
    static idx_t  keys_len    = 0;         /* number of entries */
    static idx_t  keys_cap    = 0;         /* allocated size of key table */

    /* Resize cache if necessary. */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap < n)
            new_cap *= 2;

        seq_t *new_cache = mem_resize(seq_t, cache, (size_t)new_cap);
        if (!new_cache)
            exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sig, quot;

        if (i == 1) {                   /* 1 is the only non‑prime value */
            sig = 0;
            quot = 1;
        } else {
            /* Use the provided helper functions (both return uint64_t). */
            sig  = (seq_t)omega((uint64_t)i);
            quot = (seq_t)((uint64_t)i / largest_prime_divisor((uint64_t)i));
        }

        seq_t key = cantor(sig, quot);

        /* Search for the key in the existing table. */
        idx_t id = 0;                       /* 0 means not found */
        for (idx_t k = 0; k < keys_len; ++k) {
            if (pair_keys[k] == key) {
                id = pair_ids[k];
                break;
            }
        }

        /* If not found, add a new entry. */
        if (id == 0) {
            id = keys_len + 1;              /* first index that produces this key */

            if (keys_len == keys_cap) {
                idx_t new_keys_cap = (keys_cap == 0) ? 16 : keys_cap * 2;
                seq_t *new_keys = mem_resize(seq_t, pair_keys, (size_t)new_keys_cap);
                if (!new_keys)
                    exit(EXIT_FAILURE);
                pair_keys = new_keys;

                idx_t *new_ids = (idx_t *)realloc(pair_ids,
                                                  (size_t)new_keys_cap * sizeof(idx_t));
                if (!new_ids)
                    exit(EXIT_FAILURE);
                pair_ids = new_ids;

                keys_cap = new_keys_cap;
            }

            pair_keys[keys_len] = key;
            pair_ids[keys_len]  = id;
            ++keys_len;
        }

        cache[i - 1] = (seq_t)id;
    }

    cache_len = n;
    return cache[n - 1];
}
