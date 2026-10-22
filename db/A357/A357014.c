/*
 * A357014  Numbers whose sum of exponential divisors (A051377) is odd.
 * https://oeis.org/A357014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt_u32, is_square_u32, pass_cond
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A357014: numbers whose sum of exponential divisors is odd.
   The sequence consists of all odd integers n such that every odd prime
   dividing n appears to a square exponent.  (The exponent 1 is a perfect
   square.)  This property can be tested by factoring n and checking that
   the exponent of every odd prime is a perfect square.  The function
   generates the sequence in increasing order and caches the results
   for subsequent calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/int_sqrt_u32.h"
#include "../utils/is_square_u32.h"
#include "../utils/pass_cond.h"

/* ----- Cached storage of previously computed terms ----- */
static seq_t *cache = NULL;      /* array of sequence values      */
static size_t cache_len = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;      /* allocated capacity              */
static uint64_t next_candidate = 1; /* next odd integer to test         */

/* Grow the cache array when needed.  Double its size until large enough. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) abort();        /* sequence is 1‑indexed */

    /* Ensure cache has at least n terms */
    while (cache_len < (size_t)n) {
        /* Test next odd integer */
        if (pass_cond(next_candidate)) {
            /* Add to cache */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 2;   /* skip even numbers */
    }
    return cache[n - 1];
}
