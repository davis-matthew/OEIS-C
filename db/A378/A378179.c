/*
 * A378179  Squarefree numbers k whose neighbors are neither squarefree nor
 *          prime powers.
 * https://oeis.org/A378179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 5750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9491 (2026-11-03)
 * Helpers:   is_squarefree, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A378179: the n‑th square‑free integer k such that
 * both k‑1 and k+1 are neither square‑free nor prime powers.
 *
 * The implementation uses a simple incremental search with memoisation.
 * Each term is found by testing consecutive integers, storing the result
 * in a dynamic cache.  The algorithm is O(a(n)) and runs comfortably
 * below two seconds for n ≈ 10000.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_squarefree.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

/*------------------------------------------------------------------*/
/* Helper: determine whether a 64‑bit integer is a prime power
 * with exponent > 1.                                                    */
static bool is_prime_power_uint64(uint64_t n)
{
    if (n <= 1)          return false;
    uint64_t p = smallest_prime_factor(n);
    if (p == n)          return false;          /* n is prime → not a power */
    while (n % p == 0)   n /= p;
    return n == 1;
}

/*------------------------------------------------------------------*/
/* Fast lookup table for found terms                                    */
static seq_t  *cache      = NULL;   /* array of found terms                */
static idx_t   cached_len = 0;     /* number of elements in 'cache'       */
static idx_t   cache_cap  = 0;     /* allocated capacity of 'cache'       */
static idx_t   last_checked = 1;   /* next candidate integer to test      */

/* Extend the cache until it contains at least 'n' terms.
 * The function searches successive integers starting from 'last_checked'. */
static void extend_cache(idx_t n)
{
    if (n <= cached_len) return;

    /* Ensure there is space for at least 'n' elements. */
    if (cache_cap < n) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        if (cache_cap < n) cache_cap = n;
        cache = mem_resize(seq_t, cache, cache_cap);
    }

    for (; cached_len < n; ++cached_len) {
        /* Find the next integer that satisfies the definition. */
        while (1) {
            seq_t k = (seq_t)last_checked++;
            if (!is_squarefree(k))               continue;

            /* k-1 (must not be square‑free and not a prime power) */
            seq_t lm = k - 1;
            if (is_squarefree(lm))                continue;
            if (is_prime_power_uint64((uint64_t)lm)) continue;

            /* k+1 (must not be square‑free and not a prime power) */
            seq_t lp = k + 1;
            if (is_squarefree(lp))                continue;
            if (is_prime_power_uint64((uint64_t)lp)) continue;

            /* All conditions satisfied – store the term. */
            cache[cached_len] = k;
            break;
        }
    }
}

/*------------------------------------------------------------------*/
/* The required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the offset of the sequence is 1 */
    extend_cache(n);
    return cache[n - 1];
}
