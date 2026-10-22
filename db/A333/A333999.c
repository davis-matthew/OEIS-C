/*
 * A333999  Even numbers with a Goldbach partition (p,q), p < q (p, q prime)
 *          such that q - p is a nonzero square.
 * https://oeis.org/A333999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates A333999: the nth even number (n ≥ 1) that can be written as
 * p + q with primes p < q, such that q − p is a non‑zero perfect square.
 * The algorithm scans even integers from 10 upward, testing all even square
 * differences d = 4·m².  If a valid Goldbach pair is found the number is
 * cached.  Subsequent calls reuse the cache; calls may be in any order. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* If called with n < 1, just return 0.  Offset starts at 1. */
    if (n < 1) return (seq_t)0;

    /* Static cache for the sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* Next even integer to test.  Starts at the first term in the record. */
    static uint64_t next_even = 10;
    /* Ensure the cache is large enough for further growth.  Simple doubling. */
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Search for the next valid even number. */
        while (true) {
            uint64_t E = next_even;   /* candidate even integer */
            bool found = false;
            /* Iterate over even square differences d = 4·m². */
            for (uint64_t m = 1;; ++m) {
                uint64_t d = 4ULL * m * m;
                if (d >= E) break;
                uint64_t p = (E - d) >> 1;   /* (E - d)/2 */
                uint64_t q = (E + d) >> 1;   /* (E + d)/2 */
                if (is_prime(p) && is_prime(q)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                cache[cache_len++] = (seq_t)E;   /* store result */
                next_even += 2;                  /* next candidate */
                break;                           /* go to outer loop */
            }
            next_even += 2;   /* try next even integer */
        }
    }
    return cache[n - 1];
}
