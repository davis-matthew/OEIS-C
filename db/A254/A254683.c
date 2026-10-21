/*
 * A254683  Fifth partial sums of sixth powers (A001014).
 * https://oeis.org/A254683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) +
 *            n^6 Initial values: a(1)=1, a(2)=69, a(3)=1064, a(4)=8736,
 *            a(5)=49350
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* highest index that has a value */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Grow the cache array to hold at least `needed` elements (1‑based). */
static void
ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();             /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Store the first five terms of the sequence.  They are used as the
   base values for the recurrence.  This function may be called multiple
   times safely. */
static void
init_base(void)
{
    if (cache_len >= 5) return;
    ensure_capacity(5);
    cache[1] = (seq_t)1;       /* a(1) */
    cache[2] = (seq_t)69;      /* a(2) */
    cache[3] = (seq_t)1064;    /* a(3) */
    cache[4] = (seq_t)8736;    /* a(4) */
    cache[5] = (seq_t)49350;   /* a(5) */
    cache_len = 5;
}

seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n<1, but harmless */

    ensure_capacity(n);
    if (cache_len >= n) return cache[n];

    init_base();

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t is = (seq_t)i;
        seq_t isq = is * is;
        seq_t icube = isq * is;
        seq_t isix = icube * icube;          /* i^6 */

        /* recurrence  a(i) = 5*a(i-1) - 10*a(i-2) + 10*a(i-3)
                         - 5*a(i-4) + a(i-5) + i^6            */
        seq_t val = 5 * cache[i-1] + 10 * cache[i-3] + cache[i-5] + isix;
        val -= 10 * cache[i-2];
        val -= 5  * cache[i-4];
        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
