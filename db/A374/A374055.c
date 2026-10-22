/*
 * A374055  a(1)=1, a(2)=4, and thereafter a(n) = a(n-2) + k * a(n-1), with
 *          minimal k >= 1 such that a(n) is not prime.
 * https://oeis.org/A374055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 121,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374055: a(1)=1, a(2)=4.
   For n ≥ 3, a(n) = a(n-2) + k·a(n-1) where k ≥ 1 is the smallest
   integer such that a(n) is not prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (≥ 0) */
    static size_t cache_cap = 0;   /* allocated capacity (≥ cache_len) */

    if (n == 0)          /* undefined – return 0 for safety */
        return (seq_t)0;

    /* Convert n to a size_t for indexing (assumes n ≤ SIZE_MAX) */
    size_t target = (size_t)n;
    /* target ≥ 1, so target-1 fits in size_t */

    /* Grow cache until it contains the requested term */
    while (cache_len < target) {
        /* Ensure enough storage */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            /* Realloc must be cast in C++ */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Handle the two initial values */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len++;
            continue;
        }
        if (cache_len == 1) {
            cache[1] = (seq_t)4;
            cache_len++;
            continue;
        }

        /* Compute the next term via the recurrence rule */
        seq_t prev2 = cache[cache_len - 2];
        seq_t prev1 = cache[cache_len - 1];
        seq_t candidate;
        uint64_t k = 1;

        do {
            candidate = prev2 + (seq_t)k * prev1;
            ++k;
        } while (is_prime_big(candidate));

        cache[cache_len++] = candidate;
    }

    /* Return the requested term (cache is 0‑based, n is 1‑based) */
    return cache[target - 1];
}
