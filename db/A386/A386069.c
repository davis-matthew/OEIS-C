/*
 * A386069  Primes having only {0, 3, 8, 9} as digits.
 * https://oeis.org/A386069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 58, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A386069 – primes whose decimal digits are only {0,3,8,9}.
   The function generates candidates by increasing length and tests
   primality with is_prime_big.  It caches results so any order of
   calls works. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Allowed digits.  The first digit cannot be 0, the last must be odd. */
static const uint8_t first_digits[3]  = {3, 8, 9};
static const uint8_t middle_digits[4] = {0, 3, 8, 9};
static const uint8_t last_digits[2]   = {3, 9};

/* Cache of primes found so far. */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;
/* The maximum length processed (current candidate length). */
static idx_t processed_len = 0;

/* Append a prime to the cache, resizing if necessary. */
static void push_candidate(seq_t val)
{
    if (prime_cache_cap == 0) {
        prime_cache_cap = 16;
        prime_cache = (seq_t *)malloc(sizeof(seq_t) * prime_cache_cap);
    } else if (prime_cache_len == prime_cache_cap) {
        prime_cache_cap <<= 1;
        prime_cache = (seq_t *)realloc(prime_cache,
                                      sizeof(seq_t) * prime_cache_cap);
    }
    prime_cache[prime_cache_len++] = val;
}

/* Recursively build numbers of a given length with the allowed digits. */
static void gen_digits(idx_t len, idx_t pos, seq_t cur)
{
    if (pos == len) {
        if (is_prime_big(cur))
            push_candidate(cur);
        return;
    }

    const uint8_t *digits;
    int nd;

    if (pos == 0) {                     /* first digit */
        digits = first_digits;
        nd = 3;                          /* {3,8,9} */
    } else if (pos == len - 1) {         /* last digit */
        digits = last_digits;
        nd = 2;                          /* {3,9} */
    } else {                             /* middle digit */
        digits = middle_digits;
        nd = 4;                          /* {0,3,8,9} */
    }

    for (int i = 0; i < nd; ++i) {
        seq_t next = cur * 10 + digits[i];
        gen_digits(len, pos + 1, next);
    }
}

/* Generate all candidates of a specific length. */
static void generate_with_len(idx_t len)
{
    gen_digits(len, 0, (seq_t)0);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1; behaviour here is irrelevant */

    /* Generate more candidates until we have at least n primes. */
    while (prime_cache_len < (size_t)n) {
        ++processed_len;
        generate_with_len(processed_len);
    }

    return prime_cache[n - 1];
}
