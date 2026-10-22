/*
 * A375091  First element p of sexy prime pairs (p,p+6) whose concatenation is
 *          also prime.
 * https://oeis.org/A375091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 5, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   concat_parts, is_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: First element p of sexy prime pairs (p, p+6) whose concatenation
   is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

/* Static cache for computed terms and state for prime iteration */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked_prime = 5;          /* prime before first pair */

/* Return the n-th term of the sequence, n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Ensure cache capacity */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < (size_t)n) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we reach n */
    while (cache_len < (size_t)n) {
        idx_t p = next_prime(last_checked_prime);
        last_checked_prime = p;

        if (!is_prime(p + 6))
            continue;

        seq_t concat = concat_parts(p, p + 6);
        if (is_prime_big(concat))
            cache[cache_len++] = (seq_t)p;
    }

    return cache[n - 1];
}
