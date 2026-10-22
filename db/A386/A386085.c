/*
 * A386085  Primes having only {1, 2, 3, 4} as digits.
 * https://oeis.org/A386085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal representation contains *
 *            exclusively the digits 1, 2, 3, and 4, ordered increasingly by
 *            value
 * Range:     u8 n <= 14, u16 n <= 184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_num, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/build_num.h"

/* ------------------------------------------------------------ */
/* static cache for the primes already found                   */
/* ------------------------------------------------------------ */
static seq_t *prime_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

static void cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache_cap) {
            memcpy(newp, prime_cache, cache_size * sizeof(seq_t));
            free(prime_cache);
        }
        prime_cache = newp;
        cache_cap   = new_cap;
    }
    prime_cache[cache_size++] = val;
}

/* ------------------------------------------------------------ */
/* helpers for generating the next number with digits 1‑4      */

/* state kept between calls to a(n) */
static unsigned int cur_len   = 1;      /* current digit length   */
static uint64_t   cur_limit  = 4;     /* 4^cur_len              */
static uint64_t   cur_idx    = 0;      /* next index to test     */

/* ------------------------------------------------------------ */
/* main sequence function                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; unused slot   */

    /* generate further primes until we have at least n */
    while (cache_size < n) {
        /* move to the next length when all numbers of current length are exhausted */
        if (cur_idx >= cur_limit) {
            ++cur_len;
            cur_limit *= 4;       /* 4^cur_len */
            cur_idx = 0;
        }

        seq_t cand = build_num(cur_idx, cur_len);
        if (isPrimeGeneric(cand))
            cache_append(cand);

        ++cur_idx;
    }

    return prime_cache[n - 1];
}
