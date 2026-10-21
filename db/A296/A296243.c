/*
 * A296243  Numbers k such that the multiplicative order of 2 modulo k is
 *          even.
 * https://oeis.org/A296243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th odd integer k whose multiplicative
 * order of 2 modulo k is even.  The sequence is OEIS A296243. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

/* Cached results of the sequence. */
static seq_t *cache = NULL;          /* array of terms         */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* current allocation     */

/* Last odd integer examined (kept in odd order). */
static seq_t last_candidate = 1;

/* Prime table used for factorisation. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static idx_t  prime_limit = 0;

/* Ensure that prime_list contains all primes ≤ limit.
 * The table is rebuilt each time from scratch. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt = 0;
    }
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* -- Helper: Euler's totient of a 64‑bit integer.  The argument is
 * guaranteed to fit in uint64_t for the required range of n. */
static uint64_t totient_uint64(uint64_t k)
{
    uint64_t phi = k;
    uint64_t n   = k;
    /* Make sure we have primes up to sqrt(n). */
    idx_t lim = (idx_t) (sqrt((double)n) + 1.0);
    ensure_primes_upto(lim);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) {
            phi = phi / p * (p - 1);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {
        phi = phi / n * (n - 1);
    }
    return phi;
}

/* -- Helper: checks whether the multiplicative order of 2 modulo k
 * is even.  k is odd and fits in seq_t, but we work with uint64_t
 * because all needed numbers stay below 2^64 for n ≤ 10000. */
static bool has_even_order(seq_t k)
{
    uint64_t k64 = (uint64_t)k;

    /* 1. Compute φ(k). */
    uint64_t phi = totient_uint64(k64);

    /* 2. Determine the multiplicative order of 2 modulo k.
     * Start with order = φ(k), then reduce by prime factors. */
    uint64_t order = phi;
    uint64_t temp = phi;
    idx_t lim = (idx_t) (sqrt((double)temp) + 1.0);
    ensure_primes_upto(lim);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            /* Reduce order by powers of p while possible. */
            while (order % p == 0) {
                uint64_t new_order = order / p;
                if (mod_pow((seq_t)2, new_order, k) == (seq_t)1)
                    order = new_order;
                else
                    break;
            }
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        uint64_t p = temp;
        while (order % p == 0) {
            uint64_t new_order = order / p;
            if (mod_pow((seq_t)2, new_order, k) == (seq_t)1)
                order = new_order;
            else
                break;
        }
    }

    return (order % 2ULL) == 0ULL;
}

/* Public API: return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour below offset is irrelevant */

    /* Expand cache until we have n terms. */
    while (cache_len < (size_t)n) {
        /* Find next odd candidate. */
        if (last_candidate < 3)
            last_candidate = 3;
        else
            last_candidate += 2;

        /* Test it. */
        if (has_even_order(last_candidate)) {
            /* Resize cache if needed. */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real program you would handle an OOM error. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
            }
            cache[cache_len++] = last_candidate;
        }
    }

    return cache[n - 1];
}
