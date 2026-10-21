/*
 * A277588  Numbers k such that k/10^m == 1 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 7284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive integer whose rightmost non‑zero decimal digit equals 1. */

#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamic array storing a(1),a(2),... */
static size_t cache_cap = 0;       /* allocated capacity (in elements)         */
static idx_t  cached_n = 0;        /* number of terms currently stored (last index is cached_n-1) */
static seq_t  next_candidate = 1;  /* next integer to test for the property    */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* guard, though the sequence is defined for n>=1 */

    /* Initialise cache storage on first call. */
    if (cache == NULL) {
        cache_cap = 64; /* reasonable initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have a(n). */
    while (cached_n < n) {
        /* Find the next integer whose right‑most non‑zero digit is 1. */
        for (;;) {
            seq_t t = next_candidate;
            while (t % 10 == 0) t /= 10;     /* strip trailing zeros */
            if (t % 10 == 1) break;          /* property satisfied */
            next_candidate++;
        }
        /* Ensure sufficient capacity. */
        if (cached_n >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cached_n] = next_candidate;   /* store the found term      */
        cached_n++;                         /* update counter            */
        next_candidate++;                   /* prepare for next search   */
    }

    return cache[n - 1];   /* a(1) is at index 0, a(2) at index 1, etc. */
}
