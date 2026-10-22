/*
 * A386186  Primes having only {3, 6, 8, 9} as digits.
 * https://oeis.org/A386186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n‑th prime whose decimal digits are all in {3,6,8,9}
 * Range:     u8 n <= 3, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *prime_cache = NULL;
static size_t cache_size = 0;    /* number of primes stored */
static size_t cache_cap  = 0;    /* allocated capacity */
static size_t current_len = 0;   /* current maximum length of generated numbers */
static const unsigned char digit_map[4] = {3, 6, 8, 9};

/* Append a prime to the cache, resizing if necessary */
static void cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        prime_cache = tmp;
        cache_cap = new_cap;
    }
    prime_cache[cache_size++] = val;
}

/* Recursively generate all numbers of a given length composed of {3,6,8,9} */
static void generate_len(size_t len, size_t pos, seq_t cur)
{
    if (pos == len) {
        if (isPrimeGeneric(cur))
            cache_append(cur);
        return;
    }
    for (size_t i = 0; i < 4; ++i) {
        seq_t next = cur * (seq_t)10 + (seq_t)digit_map[i];
        generate_len(len, pos + 1, next);
    }
}

/* Ensure that the cache contains at least n primes */
static void ensure_cache(idx_t n)
{
    while (cache_size < n) {
        ++current_len;
        generate_len(current_len, 0, (seq_t)0);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset not used; safety */
    ensure_cache(n);
    return prime_cache[n - 1];
}
