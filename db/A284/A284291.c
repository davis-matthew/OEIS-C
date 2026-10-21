/*
 * A284291  Primes containing a digit 6.
 * https://oeis.org/A284291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_digit_6_idx, isPrime_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime containing the decimal digit 6 by
   searching forward for primes and storing them in a growing cache.
   The function is fully memoised, works for any order of calls,
   and uses only C11/C++17 features. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/contains_digit_6_idx.h"
#include "../utils/isPrime_idx.h"

/* cache for all computed terms (64‑bit values) */
static idx_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* next candidate to test (odd numbers starting from 3) */
static idx_t next_start = 3;

/* Return the nth term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    /* Grow cache until we have at least n terms. */
    while (cache_len < n) {
        idx_t cand = next_start;

        for (;;) {
            if (!contains_digit_6_idx(cand)) {
                cand += 2;
                continue;
            }
            if (isPrime_idx(cand)) {
                /* Store the term (64‑bit) so we can convert later. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                    if (!tmp) abort(); /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
                next_start = cand + 2;
                break;  /* move on to find the next prime */
            }
            cand += 2;
        }
    }

    /* Convert the cached 64‑bit value to the required seq_t type. */
    return (seq_t)cache[n - 1];
}
