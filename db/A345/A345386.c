/*
 * A345386  Non-palindromes whose largest proper divisor is a nontrivial (not
 *          a single-digit) palindrome.
 * https://oeis.org/A345386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non‑palindrome whose largest proper divisor is a
 *            nontrivial palindrome
 * Range:     u8 n <= 7, u16 n <= 1025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome_u64.h"

/* Return the smallest prime factor of n. If n is prime, return n. */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0)   return i;
        if (n % (i + 2) == 0) return i + 2;
    }
    return n;      /* n itself is prime */
}

/* Cached sequence storage */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 110;   /* first candidate */

/* External function required by the build system */
seq_t a(idx_t n)
{
    /* Ensure cache is allocated on the first call */
    if (cache == NULL) {
        cache_cap = 512;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Generate terms until the cache contains at least n terms */
    size_t need = (size_t)n;
    while (cache_len < need) {
        /* Search for the next term */
        while (true) {
            uint64_t cand64 = next_candidate++;      /* candidate integer */
            seq_t cand = cand64;                     /* convert to seq_t */

            /* 1. Candidate must NOT be a palindrome */
            if (is_palindrome_u64(cand64)) continue;

            /* 2. Compute smallest prime factor and the largest proper divisor */
            uint64_t spf = smallest_prime_factor(cand64);
            if (spf <= 1 || spf == cand64) continue;  /* prime or 1 */
            uint64_t div64 = cand64 / spf;
            if (div64 < 10) continue;                 /* needs at least two digits */

            /* 3. The largest proper divisor must be a palindrome */
            if (!is_palindrome_u64(div64)) continue;

            /* Term found – store it in the cache */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
            break;  /* go back to generate next term if needed */
        }
    }
    return cache[need - 1];
}
