/*
 * A353548  Expansion of e.g.f. -log(1-4*x) * exp(x)/4.
 * https://oeis.org/A353548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353548
   Expansion of e.g.f. -log(1-4*x) * exp(x)/4
   a(0) = 0
   a(1) = 1
   a(n) = (4*n-3)*a(n-1) - 4*(n-1)*a(n-2) + 1   for n >= 2
   Memoised to allow any order of calls. */
#include <stdlib.h>
#include <assert.h>

/* The sequence array holds pre‑computed values.
   The array may be enlarged on demand. */
static seq_t *cache = NULL;        /* pointer to cache array          */
static idx_t cache_size = 0;       /* allocated entries (capacity)    */
static idx_t last_computed = 0;    /* highest n that has been produced */

static void ensure_capacity(idx_t n)
{
    /* n is the maximum index we will need (inclusive). */
    if (n + 1 <= cache_size)
        return;

    idx_t newcap = cache_size ? cache_size : 4;
    while (newcap <= n)
        newcap <<= 1;               /* double until we exceed n        */

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)newcap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* out of memory */
    cache = tmp;
    cache_size = newcap;
}

/* Public function computing a(n).  The function is side‑effecting
   only the static cache and is fully re‑entrant for any call order. */
seq_t a(idx_t n)
{
    /* initialise cache lazily on first call */
    if (!cache) {
        ensure_capacity(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
        last_computed = 1;
    }

    /* nothing to do if requested value already known */
    if (n <= last_computed)
        return cache[n];

    /* make sure there is room for all needed indices */
    ensure_capacity(n);

    /* compute successive terms until we reach n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(4 * i - 3);          /* (4*i - 3) */
        seq_t coeff2 = (seq_t)(4 * (i - 1));        /* 4*(i-1)   */

        /* recurrence: a(i) = coeff1*a(i-1) - coeff2*a(i-2) + 1 */
        seq_t t1 = coeff1 * cache[i - 1];
        seq_t t2 = coeff2 * cache[i - 2];
        cache[i] = t1 - t2 + (seq_t)1;
    }

    last_computed = n;
    return cache[n];
}
