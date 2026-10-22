/*
 * A377166  Expansion of g.f. x*(21 + 123*x + 129*x^2 + 4*x^3 + 129*x^4 +
 *          123*x^5 + 21*x^6)/((1 - x)^3*(1 + x + x^2 + x^3)^2).
 * https://oeis.org/A377166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) - a(n-8) + a(n-9) for n>8
 *            Initial values: a(0)=0, a(1)=21, a(2)=144, a(3)=273, a(4)=277,
 *            a(5)=448, a(6)=817, a(7)=1096, a(8)=1104
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *cache   = NULL;   /* array of computed terms                */
static size_t cache_cap = 0;    /* allocated size (number of elements)    */
static idx_t   cache_max = 0;   /* highest index that has been computed   */

/* make sure the cache can hold indices up to `need` (inclusive) */
static void
ensure_cache_cap(idx_t need)
{
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = (size_t)need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();             /* out of memory → terminate */
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* initialise the cache once with the nine known seed values */
static void
init_cache_if_needed(void)
{
    if (cache != NULL) return;           /* already done */

    /* need space for indices 0..8  */
    ensure_cache_cap(8);

    cache[0] = 0;
    cache[1] = 21;
    cache[2] = 144;
    cache[3] = 273;
    cache[4] = 277;
    cache[5] = 448;
    cache[6] = 817;
    cache[7] = 1096;
    cache[8] = 1104;

    cache_max = 8;
}

/* compute terms up to index `n` (assumes n > cache_max) */
static void
build_until(idx_t n)
{
    /* ensure we have enough storage */
    ensure_cache_cap(n);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* use seq_t arithmetic; for fixed‑width types the intermediate
           extension to __int128 guarantees no overflow on the usual
           64‑bit builds; for arbitrary‑precision types the operations are
           performed exactly. */
        seq_t val = cache[i-1]
                  + ((seq_t)2) * cache[i-4]
                  - ((seq_t)2) * cache[i-5]
                  - cache[i-8]
                  + cache[i-9];
        cache[i] = val;
    }
    cache_max = n;
}

/* Public entry point */
seq_t
a(idx_t n)
{
    init_cache_if_needed();
    if (n > cache_max)
        build_until(n);
    return cache[n];
}
