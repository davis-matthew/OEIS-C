/*
 * A342717  Primes q such that 15*q-4, 15*q-2, 15*q+2 and 15*q+4 are all
 *          primes.
 * https://oeis.org/A342717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A342717: primes q such that 15*q-4, 15*q-2, 15*q+2, 15*q+4 are all primes.
   The function returns the n‑th term of the sequence (1‑based indexing). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_q = 3;

/* Generate terms until the array contains at least `target` elements. */
static void generate_to(idx_t target)
{
    while (cache_len < target) {
        if (is_prime_big(next_q)) {
            seq_t t = mul_small(next_q, 15);
            if (is_prime_big(t - 4) &&
                is_prime_big(t - 2) &&
                is_prime_big(t + 2) &&
                is_prime_big(t + 4)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1); /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_q;
            }
        }
        next_q += 2;  /* only odd candidates */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for offset 1, return 0 as sentinel */
    if (cache_len < (size_t)n)    /* generate more terms if needed */
        generate_to(n);
    return cache[(size_t)n - 1];
}
