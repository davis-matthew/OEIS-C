/*
 * A357758  Numbers k such that in the binary expansion of k, the Hamming
 *          weight of each block differs by at most 1 from every other block
 *          of the same length.
 * https://oeis.org/A357758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   local_bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A357758: numbers k whose binary blocks of equal length have
   Hamming weights differing by at most 1. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/local_bitlen_seq_t.h"

/* ----- local utilities ---------------------------------------------------- */

/* ----- property test ------------------------------------------------------ */

/* Return true iff k satisfies the property of A357758. */
static bool is_valid_k(seq_t k)
{
    unsigned len = local_bitlen_seq_t(k);

    /* Allocate working space. The maximum len for 64‑bit types is 64;
       for larger integer types the required space is still modest. */
    unsigned char *bits = (unsigned char *)malloc(len);
    size_t        *pref = (size_t *)malloc((len + 1) * sizeof(size_t));
    if (!bits || !pref) {
        free(bits);
        free(pref);
        return false;          /* Treat allocation failure as failure.  */
    }

    /* Extract bits (least significant first). */
    for (unsigned i = 0; i < len; ++i)
        bits[i] = (unsigned char)((k >> i) & 1);

    /* Prefix sums of bits. */
    pref[0] = 0;
    for (unsigned i = 0; i < len; ++i)
        pref[i + 1] = pref[i] + bits[i];

    /* Test all block lengths. */
    bool ok = true;
    for (unsigned L = 1; L <= len - 1 && ok; ++L) {
        unsigned minW = (unsigned)-1;
        unsigned maxW = 0;
        for (unsigned i = 0; i <= len - L; ++i) {
            unsigned w = (unsigned)(pref[i + L] - pref[i]);
            if (w < minW) minW = w;
            if (w > maxW) maxW = w;
            if (maxW - minW > 1) { ok = false; break; }
        }
    }

    free(bits);
    free(pref);
    return ok;
}

/* ----- memoised generation ----------------------------------------------- */

static seq_t *cache       = NULL;   /* stored terms */
static size_t cache_cap   = 0;     /* allocated capacity */
static size_t cache_len   = 0;     /* number of terms stored */
static seq_t next_to_check = 0;    /* next candidate to test */

/* Grow cache array when capacity exhausted. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* unrecoverable failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Find next valid term. */
        while (!is_valid_k(next_to_check))
            next_to_check++;

        /* Store it. */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = next_to_check;

        /* Advance to next candidate for following search. */
        ++next_to_check;
    }
}

/* Public sequence function. */
extern seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 – return 0 */

    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
