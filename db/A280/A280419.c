/*
 * A280419  Primes in A132934.
 * https://oeis.org/A280419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, bref
 * Formula:   a(n) is the concatenation of 1 and the successive composite
 *            integers until the resulting number is prime
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   digits, is_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_comp = 4;      /* next integer to examine for a composite */
static seq_t current      = 1;   /* value that will be extended with more composites */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset‑1 sequence, undefined for n==0 */

    /* If the result was already computed, return it */
    if (n <= cache_len) return cache[n - 1];

    /* Allocate or grow the cache array */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len < n) {
        seq_t val = current;          /* start extending the current concatenation */

        while (1) {
            /* find the next composite number */
            while (is_prime(next_comp)) ++next_comp;

            /* append the composite to the current value */
            seq_t powd  = pow10(digits(next_comp));
            val = val * powd + (seq_t)next_comp;
            ++next_comp;

            /* check if the new value is prime */
            if (is_prime_big(val)) {
                /* store the term in the cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
                current = val;          /* next term will continue from here */
                break;                  /* move on to compute the next term */
            }
        }
    }
    return cache[n - 1];
}
