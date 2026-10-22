/*
 * A346652  Hexagonal pandigitals.
 * https://oeis.org/A346652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A346652: a(n) = the n-th hexagonal number that contains every decimal
   digit 0–9 at least once (hexagonal pandigitals). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Check whether a decimal representation of v contains all ten digits. */
static bool has_all_digits(seq_t v)
{
    unsigned mask = 0;
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        mask |= 1U << d;
        if (mask == 0x3FFU)   /* all 10 bits set */
            return true;
        v /= 10;
    }
    return mask == 0x3FFU;
}

/* Dynamic cache of found terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  last_k     = 0;   /* last hexagonal index examined */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < need)
        new_cap = need;
    if (cache == NULL) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    cache_cap = new_cap;
}

/* Main function: return the n-th term of the sequence (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; value 0 is unused */

    /* Generate terms until we have at least n of them. */
    if (cache_len < (size_t)n) {
        idx_t k = last_k + 1;
        while (cache_len < (size_t)n) {
            seq_t two_k = (seq_t)k * 2;
            seq_t val   = (seq_t)k * (two_k - 1);  /* H_k = k(2k-1) */

            if (has_all_digits(val)) {
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = val;
            }
            ++k;
        }
        last_k = k - 1;
    }
    return cache[n - 1];
}
