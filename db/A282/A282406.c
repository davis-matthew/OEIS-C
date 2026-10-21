/*
 * A282406  Primes p = x^2 + y^2 such that x - y is a square greater than one.
 * https://oeis.org/A282406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 59, u32 n <= 855, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A282406: primes p = x² + y² with x−y a perfect square > 1
   The algorithm searches prime by prime, uses Cornacchia's algorithm to
   find the representation p = x² + y², tests the difference condition,
   and stores each qualifying prime in a static cache.  Subsequent calls
   retrieve the result from the cache, guaranteeing O(1) answering after
   the first expansion. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <utility>
#endif

/* Utilities provided by the OEIS build environment */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t start); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/floor_sqrt.h"    /* seq_t floor_sqrt(seq_t x); */

/* static cache to store found primes */
static seq_t *cache = NULL;          /* dynamic array of found primes   */
static size_t cache_len = 0;         /* number of entries in cache      */
static size_t cache_cap = 0;         /* current capacity of the array   */
static uint64_t last_checked = 2;    /* last prime examined */

/* Reallocate the cache array to a larger capacity.  Works for both C
   (plain types) and C++ (objects with non‑trivial constructors). */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    void *p = NULL;
#ifdef __cplusplus
    p = ::operator new[](new_cap * sizeof(seq_t));
#else
    p = malloc(new_cap * sizeof(seq_t));
#endif
    if (!p) abort();

    seq_t *new_arr = (seq_t*)p;
#if defined(__cplusplus)
    /* Move existing elements into the new array. */
    for (size_t i = 0; i < cache_len; ++i) {
        new (&new_arr[i]) seq_t(std::move(cache[i]));
        cache[i].~seq_t();     /* destroy old element */
    }
    if (cache) {
        ::operator delete[](cache);   /* free old storage */
    }
#else
    memcpy(new_arr, cache, cache_len * sizeof(seq_t));
    if (cache) free(cache);
#endif
    cache = new_arr;
    cache_cap = new_cap;
}

/* Core routine: returns the nth term (n ≥ 1).  No external state is
   modified other than the internal cache. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Ensure the cache contains at least n entries. */
    while (cache_len < (size_t)n) {
        /* Find next prime ≥ last_checked + 1 */
        last_checked = next_prime(last_checked + 1);

        /* Only primes ≡ 1 (mod 4) can be expressed as x² + y². */
        if ((last_checked & 3) != 1) continue;

        seq_t p = (seq_t)last_checked;  /* cast to seq_t */

        /* Find a quadratic non‑residue a modulo p. */
        seq_t a_candidate = 2;
        seq_t nxt;
        uint64_t exp_half = ((uint64_t)p - 1ULL) / 2ULL;
        while (1) {
            nxt = mod_pow(a_candidate, exp_half, p);
            if (nxt == p - 1) break; /* found a^(p−1)/2 ≡ –1 (mod p) */
            a_candidate++;
        }

        /* Compute t = a^((p−1)/4) mod p, a √(–1) modulo p. */
        uint64_t exp_quart = ((uint64_t)p - 1ULL) / 4ULL;
        seq_t t = mod_pow(a_candidate, exp_quart, p);

        /* Cornacchia's algorithm to obtain x and y with p = x² + y². */
        seq_t x = p, y = t;
        while (y * y > p) {
            seq_t r = x % y;
            x = y;
            y = r;
        }
        seq_t x1 = y;
        seq_t y1_sq = p - x1 * x1;
        if (y1_sq < 0) continue;           /* safety check */

        seq_t y1 = floor_sqrt(y1_sq);
        if (y1 * y1 != y1_sq) continue;    /* not a perfect square */

        /* Arrange so that X ≥ Y for difference calculation. */
        seq_t X = x1, Y = y1;
        if (Y > X) { seq_t tmp = X; X = Y; Y = tmp; }

        seq_t diff = X - Y;
        if (diff <= 1) continue;           /* need square > 1 */

        seq_t root = floor_sqrt(diff);
        if (root * root != diff) continue; /* difference not a perfect square */

        /* Store the qualifying prime in cache. */
        if (cache_len == cache_cap) grow_cache();
        cache[cache_len++] = p;
    }

    return cache[(size_t)n - 1];
}
