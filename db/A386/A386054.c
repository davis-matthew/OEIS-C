/*
 * A386054  Primes having only {0, 2, 7, 9} as digits.
 * https://oeis.org/A386054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 47, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:  Primes whose decimal digits are only 0,2,7,9
   The function computes the n-th such prime (1‑based indexing).  */

/* required utility headers */
#include <stddef.h>
#include <stdint.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

/* static data for computation and caching */
static seq_t *cache   = NULL;   /* array of found primes           */
static size_t cache_len = 0;    /* number of primes stored         */
static size_t cache_cap = 0;    /* current allocation capacity     */
static idx_t  next_len  = 1;    /* next digit length to generate   */

/* digit sets used in generation */
static const uint8_t first_digits[3] = {2, 7, 9};   /* first digit cannot be 0 */
static const uint8_t other_digits[4] = {0, 2, 7, 9};

/* recursive helper to generate numbers of a fixed length */
static void generate_rec(idx_t pos, seq_t val, idx_t len)
{
    if (pos == len) {
        if (is_prime_big(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        seq_t next = val * 10 + other_digits[i];
        generate_rec(pos + 1, next, len);
    }
}

/* generate all primes of exactly 'len' decimal digits consisting of
   allowed digits and store them in the cache */
static void generate_len(idx_t len)
{
    if (len == 1) {
        for (int i = 0; i < 3; ++i) {
            seq_t v = first_digits[i];
            if (is_prime_big(v))
                append_candidate(&cache, &cache_len, &cache_cap, v);
        }
        return;
    }
    for (int i = 0; i < 3; ++i) {
        seq_t start = first_digits[i];
        generate_rec(1, start, len);
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 gracefully */

    /* extend cache until it contains at least n primes */
    while ((idx_t)cache_len < n) {
        generate_len(next_len);
        ++next_len;
    }
    return cache[n - 1];
}
