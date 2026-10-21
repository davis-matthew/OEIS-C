/*
 * A249624  Numbers n such that the sum of n and the least prime>n
 *          (A151800(n)) is prime.
 * https://oeis.org/A249624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A249624: numbers n such that n + the least prime > n
   is prime.  The function returns the n-th such number (1‑based indexing). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_tested = 0;
    static bool initialized = false;

    if (!initialized) {
        /* initial allocation */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_tested = 0;
        initialized = true;
    }

    /* grow cache to have at least n elements */
    while (cache_len < n) {
        uint64_t cand = last_tested;
        uint64_t q = next_prime(cand);                     /* least prime > cand */
        uint64_t sum = cand + q;
        if (is_prime_big((seq_t)sum)) {                    /* cand qualifies */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cand;
        }
        last_tested = cand + 1;                            /* next candidate */
    }

    return cache[n - 1];
}
