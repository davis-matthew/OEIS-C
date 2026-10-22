/*
 * A337508  Primes such that neither the left half nor the right half of the
 *          prime is prime.
 * https://oeis.org/A337508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A337508: primes such that neither the left half nor the
   right half (ignoring the middle digit for odd-length numbers)
   is prime.  The function builds a cache of terms and expands it
   lazily until the requested index is reached. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility routines supplied by the build environment.  Only the
   ones actually used are included. */
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Cache of known terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next prime candidate to test.  11 is the first prime that can
   satisfy the conditions. */
static seq_t next_candidate = 11;

/* Append a new value to the cache, growing it if necessary. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* unrecoverable error */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main routine: return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but avoid UB */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* A prime must be odd (except 2 which is excluded). */
        if (next_candidate == 2) {
            next_candidate = 3;
        }

        /* Test primality of the candidate. */
        if (is_prime_big(next_candidate)) {
            /* Length of the decimal representation. */
            int len = digit_len(next_candidate);
            int half = len / 2;

            seq_t left, right;

            if (len & 1) { /* odd number of digits – skip the middle digit */
                seq_t denom1 = pow10(half + 1);
                seq_t denom2 = pow10(half);
                left  = next_candidate / denom1;
                right = next_candidate % denom2;
            } else {        /* even number of digits */
                seq_t denom = pow10(half);
                left  = next_candidate / denom;
                right = next_candidate % denom;
            }

            /* Both halves must be non‑prime. */
            if (!is_prime_big(left) && !is_prime_big(right)) {
                cache_push(next_candidate);
            }
        }

        next_candidate++;
    }

    return cache[n - 1];
}
