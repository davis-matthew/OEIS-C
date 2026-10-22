/*
 * A347344  Positive integers k such that k with the first (most significant)
 *          digit repeated is prime.
 * https://oeis.org/A347344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that D(k) = k with its
 *            most significant digit repeated is prime, where D(k) = d·10^m +
 *            k, d = first digit of k and m = number of digits of k
 * Range:     u8 n <= 5, u16 n <= 847, u32 n <= 209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/digits_of.h"
#include "../utils/pow10.h"

/* Helper to compute the most significant digit of a uint64_t value. */
static unsigned leading_digit(uint64_t x)
{
    while (x >= 10) x /= 10;
    return (unsigned)x;
}

/* Cached results of the sequence. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* The largest k that has been tested. */
static uint64_t tested_k = 1;

/* Grow the cache buffer if necessary. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    if (cache) {
        memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Append a new element to the cache. */
static void cache_append(seq_t v)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset 1; n>=1 in normal use */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t k = tested_k++;

        unsigned d = leading_digit(k);
        int dlen   = digits_of((idx_t)k);          /* number of digits of k */
        seq_t D    = (seq_t)d * pow10((idx_t)dlen) + (seq_t)k;

        if (is_prime_big(D)) {
            cache_append((seq_t)k);
        }
    }
    return cache[(size_t)(n - 1)];
}
