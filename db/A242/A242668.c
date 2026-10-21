/*
 * A242668  Expansion of 1/(1 - 8*x + 16*x^2 + x^4 - 4*x^5).
 * https://oeis.org/A242668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A242668: a(n) = 4*a(n-1) - a(n-4) + 4^n   (n >= 4)
   a(0)=1, a(1)=8, a(2)=48, a(3)=256
   (derived from the generating function 1/((1-4x)(1-4x+x^4))) */

#include <stdlib.h>
#include <stddef.h>

/* static cache for computed terms */
static seq_t *cache = NULL;   /* array of a(k) values */
static size_t cap   = 0;      /* allocated size of cache   */
static size_t len   = 0;      /* number of elements stored */

/* ensure cache has room for indices up to needed (inclusive) */
static void
ensure_capacity(size_t needed)
{
    if (needed < cap)
        return;
    size_t new_cap = cap ? cap * 2 : 4;
    while (new_cap <= needed)
        new_cap <<= 1;               /* double until large enough */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

/* initialise cache with the first four terms */
static void
init_cache(void)
{
    if (cache != NULL)
        return;                     /* already initialised */

    ensure_capacity(3);              /* need indices 0..3 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)8;
    cache[2] = (seq_t)48;
    cache[3] = (seq_t)256;
    len = 4;
}

seq_t
a(idx_t n)
{
    init_cache();

    size_t idx = (size_t)n;
    if (idx < len)
        return cache[idx];

    /* grow cache up to the requested index */
    ensure_capacity(idx);

    /* compute 4^i on the fly, start with 4^len */
    seq_t pow4 = (seq_t)1;
    for (size_t i = 0; i < len; ++i)
        pow4 *= (seq_t)4;            /* pow4 = 4^len */

    /* fill in new terms */
    for (size_t i = len; i <= idx; ++i) {
        if (i < 4) {                 /* safety for small indices */
            /* base values already set, skip */
        } else {
            cache[i] = cache[i - 1] * (seq_t)4
                         - cache[i - 4]
                         + pow4;
        }
        pow4 *= (seq_t)4;             /* advance 4^i */
    }

    len = idx + 1;
    return cache[idx];
}
