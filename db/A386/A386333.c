/*
 * A386333  Primes without {1, 7} as digits.
 * https://oeis.org/A386333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1431, u32 n <= 55, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386333: primes that do not contain the digits 1 or 7 */
/* This implementation searches for primes incrementally and caches the results */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* provides is_prime_big */

/* Test if a number contains digit 1 or 7. */
static inline bool has_forbidden_digit(seq_t x)
{
    while (x > 0)
    {
        uint8_t d = (uint8_t)(x % (seq_t)10);
        if (d == 1 || d == 7)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

/* Cache of found primes. */
static seq_t *prime_cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t last_checked = 1;   /* last integer examined */

/* Ensure enough capacity for the cache. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n. */
    while (cache_len < (size_t)n)
    {
        seq_t cand = ++last_checked;   /* next integer candidate */
        if (has_forbidden_digit(cand))
            continue;
        if (is_prime_big(cand))
        {
            ensure_capacity(cache_len + 1);
            prime_cache[cache_len++] = cand;
        }
    }
    return prime_cache[n - 1];
}
