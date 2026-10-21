/*
 * A291747  Nonprimes of the form 7*k + 1.
 * https://oeis.org/A291747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer that is NOT prime and satisfies
 *            a(n) ≡ 1 (mod 7). a(1) = 1
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t n) */

static seq_t *cache      = NULL;
static size_t    cache_len = 0;
static size_t    cache_cap = 0;
static idx_t     next_candidate = 1;

seq_t a(idx_t n)
{
    /* Generate terms until the cache holds at least n entries */
    while (cache_len < n) {
        /* Move to the next integer congruent to 1 modulo 7 */
        while (next_candidate % 7 != 1)
            ++next_candidate;

        /* Determine whether the number is a non‑prime */
        bool is_nonprime;
        if (next_candidate == 1)            /* 1 is not prime by definition */
            is_nonprime = true;
        else
            is_nonprime = !is_prime(next_candidate);

        if (is_nonprime) {
            /* Ensure the cache has space for a new entry */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* Advance to the following integer for the next loop iteration */
        ++next_candidate;
    }

    return cache[n - 1];
}
