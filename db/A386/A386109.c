/*
 * A386109  Primes having only {1, 3, 4, 9} as digits.
 * https://oeis.org/A386109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate primes whose decimal digits belong to {1,3,4,9}
   by generating all numbers of increasing length and testing
   primality with isPrimeGeneric.  The sequence is thus built
   in numeric order and cached for future calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static const uint8_t allowed_digits[4] = {1, 3, 4, 9};

/* Cached primes.  They are stored in increasing numeric order. */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of cached primes */

/* Largest length already generated.  All lengths ≤ this are fully processed. */
static int max_len_gen = 0;

/* Append a prime to the cache, resizing if necessary. */
static void add_prime(seq_t prime)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = prime;
}

/* Recursively generate all numbers of a given length, testing each leaf for primality. */
static void generate_len_recursive(int pos, int target_len, seq_t cur)
{
    if (pos == target_len) {
        if (isPrimeGeneric(cur)) add_prime(cur);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        uint8_t d = allowed_digits[i];
        /* Skip numbers ending in 4 – they are even and >2. */
        if (pos == target_len - 1 && d == 4) continue;
        seq_t next = cur * (seq_t)10 + (seq_t)d;
        generate_len_recursive(pos + 1, target_len, next);
    }
}

/* Ensure that at least 'needed' primes are cached. */
static void ensure_generated(idx_t needed)
{
    if (needed <= cache_len) return;
    for (int len = max_len_gen + 1; cache_len < needed; ++len) {
        generate_len_recursive(0, len, (seq_t)0);
        max_len_gen = len;
    }
}

/* OEIS sequence function a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1 */
    ensure_generated(n);
    return seq_cache[n - 1];
}
