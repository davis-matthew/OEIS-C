/*
 * A373992  Numbers k such that A328768(k) is a multiple of 3, where A328768
 *          is the first primorial based variant of arithmetic derivative.
 * https://oeis.org/A373992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 9467, u32 n <= 9467, u64 n <= 9467,
 *            u128 n <= 9467, big any n
 * Helpers:   has_v2_multiple_of_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373992: numbers k such that the first‑primorial‑based
 * arithmetic derivative A328768(k) is a multiple of 3.
 *
 * Equivalently, k is 0, or k ≡ 0 (mod 9), or k is not divisible by 3
 * and the exponent of 2 in k is a multiple of 3.
 *
 * The function returns the n‑th term (a(1)=0) with a memoised static
 * cache.  It scans successive integers, testing the above condition,
 * adding qualifying numbers to the cache until it contains at least
 * n elements.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_v2_multiple_of_three.h"

/* ---------- Static cache state ------------------------------------ */
static seq_t *cache      = NULL;      /* array of cached terms   */
static size_t cache_cap  = 0;         /* allocated size          */
static size_t cache_len  = 0;         /* number of terms filled  */
static uint64_t next_candidate = 1;   /* next integer to test    */
static bool zero_added = false;       /* whether the term 0 is stored */

/* Ensure the cache can hold at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_cap2 = cache_cap ? cache_cap * 2 : 16;
    while (new_cap2 < new_cap)
        new_cap2 *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap2 * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap2;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Build the cache up to the requested index. */
    while (cache_len < (size_t)n) {
        /* Insert the initial 0 term if not yet done. */
        if (!zero_added) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)0;
            zero_added = true;
            continue;   /* loop back for the next position   */
        }

        /* Test the next integer candidate. */
        uint64_t k = next_candidate;
        bool good = false;

        if ((k % 9ULL) == 0ULL)
            good = true;
        else if ((k % 3ULL) != 0ULL)
            good = has_v2_multiple_of_three(k);

        if (good) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }

        ++next_candidate;   /* advance to the next integer to test */
    }

    return cache[(size_t)n - 1];
}
