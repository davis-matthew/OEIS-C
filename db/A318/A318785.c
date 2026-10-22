/*
 * A318785  Numbers which are prime if each digit is replaced by its 9's
 *          complement.
 * https://oeis.org/A318785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th natural number whose 9's complement is prime
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   compl_of, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/compl_of.h"

/* Static cache for previously computed terms. */
static seq_t *cache = NULL;   /* array of terms */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of terms stored */
static idx_t last_checked = 0;/* last integer tested for the property */

/* Grow the cache array to hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc may fail; keep the old cache on failure. */
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1; harmless */

    if (!cache) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        cache_cap = 16;
        cache_len = 0;
        last_checked = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (cache_len + 1 > cache_cap)
            ensure_cache_capacity(cache_len + 1);

        idx_t cand = last_checked + 1;
        seq_t comp = compl_of(cand);

        /* The 9's complement must be prime. */
        if (isPrimeGeneric(comp))
            cache[cache_len++] = (seq_t)cand;

        last_checked = cand;
    }

    return cache[n - 1];
}
