/*
 * A394747  Numbers k such that k-4 and k+4 are prime and gcd(k, sigma(k)) is
 *          prime.
 * https://oeis.org/A394747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A394747:
   a(n) is the nth integer k (k ≥ 15) for which
      - k−4 and k+4 are prime,
      - gcd(k, σ(k)) is prime.
   The algorithm searches k = 3 + 6i (k ≡ 3 (mod 6)),
   testing the conditions and storing found values in a
   dynamically resized cache so that repeated calls are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current capacity of the array */

static idx_t next_k = 3;         /* next candidate k (k ≡ 3 (mod 6)) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, safeguard */

    /* Expand the cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t k = (seq_t)next_k;

        /* only candidates ≥ 9 have k‑4 ≥ 5; otherwise subtraction
           would underflow for unsigned types */
        if (next_k >= 9) {
            seq_t k_minus4 = k - (seq_t)4;
            seq_t k_plus4  = k + (seq_t)4;

            if (is_prime_big(k_minus4) && is_prime_big(k_plus4)) {
                seq_t sigma = sigma_of((idx_t)k);
                seq_t g     = gcd(k, sigma);
                if (is_prime_big(g)) {
                    /* Grow the cache if full */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *new_cache = (seq_t *)realloc(cache,
                                                             new_cap * sizeof(seq_t));
                        if (!new_cache) abort();
                        cache = new_cache;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = k;
                }
            }
        }

        next_k += 6;   /* next candidate (always divisible by 3) */
    }

    return cache[n - 1];
}
