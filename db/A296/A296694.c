/*
 * A296694  Numbers whose base-4 digits d(m), d(m-1), ... d(0) have #(rises) =
 *          #(falls); see Comments.
 * https://oeis.org/A296694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = nth integer whose base‑4 digits d(m)…d(0) satisfy
   #(rises) = #(falls).  The implementation performs a brute‑force
   search with memoisation.  The first n values are cached so that
   subsequent calls to a(n) are O(1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* helper: does x have an equal number of rises and falls in base‑4? */
static inline bool balanced_rises_falls(seq_t x)
{
    /* single digit numbers are trivially balanced */
    if (x == 0) return true;

    /* a word of 256 base‑4 digits is sufficient for the tests */
    uint8_t d[256];
    idx_t len = 0;
    seq_t t = x;

    /* extract base‑4 digits (least significant first) */
    while (t > 0) {
        d[len++] = (uint8_t)(t % 4);
        t /= 4;
    }

    /* count rises and falls (most significant to least).  If len<2
       the counts are both zero, so the return value will be true. */
    int rises = 0, falls = 0;
    for (idx_t i = len; i > 1; --i) {
        uint8_t a = d[i - 1];
        uint8_t b = d[i - 2];
        if (a < b) {
            ++rises;
        } else if (a > b) {
            ++falls;
        }
    }
    return rises == falls;
}

/* --- memoisation storage --- */
static seq_t *cache = NULL;     /* array of found numbers               */
static size_t cache_len = 0;    /* number of values currently cached    */
static size_t cache_cap = 0;    /* current allocated capacity           */
static idx_t  current   = 1;    /* next integer to test                 */

/* ensure more space for the cache */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for 0; return 0 as a placeholder */
        return 0;

    /* grow cache until we have at least n values */
    while (cache_len < n) {
        seq_t val = (seq_t)current;
        if (balanced_rises_falls(val)) {
            if (cache_len >= cache_cap)
                grow_cache();
            cache[cache_len++] = val;
        }
        ++current;
    }
    return cache[n - 1];
}
