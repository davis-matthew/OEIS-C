/*
 * A290402  Primes congruent to {7, 17} mod 24.
 * https://oeis.org/A290402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning integers congruent to 7 or 17 (mod 24)
   and testing each for primality. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

/* cached primes */
static seq_t *cache   = NULL;      /* array of found primes          */
static size_t cache_cap = 0;       /* allocated capacity             */
static size_t cache_size = 0;      /* number of primes stored        */

/* candidate generator state */
static idx_t candidate = 7;        /* next number to test (7,17,31,41,…) */
static bool  step_ten  = true;     /* true → +10, false → +14 */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) {          /* allocation failure */
            cache   = NULL;
            cache_cap = 0;
            return;
        }
        cache   = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but harmless for n>=1 */

    /* Generate primes until we have at least n of them. */
    while (cache_size < n) {
        if (is_prime_small((seq_t)candidate)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)candidate;
        }

        /* Advance candidate: +10, +14, +10, +14, … */
        candidate += step_ten ? (idx_t)10 : (idx_t)14;
        step_ten = !step_ten;
    }

    return cache[n - 1];
}
