/*
 * A246449  Numbers k such that no cube can end in k (in the sense of the
 *          respective decimal expansions).
 * https://oeis.org/A246449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth positive integer that cannot be the trailing decimal digits of any perfect cube */

#include <stdint.h>
#include <stdlib.h>

/* cache of sequence values, built lazily */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements stored */
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t current_len = 1;  /* digits processed so far, 1 → no terms */

/* grow cache to hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* memory failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* generate all d‑digit numbers that are not cube residues modulo 10^d
   and append them to the cache */
static void generate_len(idx_t len)
{
    /* bounds for d‑digit numbers */
    idx_t low = 1;
    for (idx_t i = 1; i < len; ++i) low *= 10;
    idx_t high  = low * 10 - 1;
    idx_t base  = high + 1;          /* 10^len */

    /* find all residues of x^3 (mod 10^len) */
    unsigned char *vis = (unsigned char*)calloc((size_t)base, 1);
    if (!vis) exit(1);

    unsigned __int128 bb = base;
    for (idx_t m = 0; m < base; ++m) {
        unsigned __int128 mm = m;
        unsigned __int128 r  = ((mm * mm) % bb) * mm % bb;
        vis[(size_t)r] = 1;
    }

    /* numbers that are NOT residues become sequence terms */
    for (idx_t k = low; k <= high; ++k) {
        if (!vis[(size_t)k]) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    free(vis);
}

/* external interface */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        ++current_len;
        generate_len(current_len);
    }
    return cache[n - 1];
}
