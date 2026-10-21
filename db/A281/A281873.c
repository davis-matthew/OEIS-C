/*
 * A281873  a(n+1) is the smallest number greater than a(n) such that
 *          Sum_{j=1..n+1} 1/a(j) <= 4, a(1) = 1.
 * https://oeis.org/A281873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1)=1. For k≥1 let Rnum,Rden be the numerator and denominator
 *            of the residual R = 4 − Σ_{j=1..k}1/a(j) (so initially
 *            Rnum=4,Rden=1). The next term is b = max( a(k)+1 , ceil( Rden /
 *            Rnum ) ) (1) which guarantees that after inserting 1/b the
 *            residual stays non‑negative. Update the residual as Rnum...
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 33, u128 n <= 34,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* cache of computed a(i) */
static size_t cache_cap;             /* cache capacity */
static size_t cache_len = 0;         /* number of terms stored */
static seq_t prev_val = 0;           /* last computed a(n) */
static seq_t Rnum = 0;               /* numerator of residual */
static seq_t Rden = 1;               /* denominator of residual */

/* Initialise the cache and residual on first use. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        prev_val = 0;
        Rnum = 4;
        Rden = 1;
    }
}

/* Increase cache capacity to hold at least ‘need’ elements. */
static void grow_cache(size_t need)
{
    size_t cap = cache_cap;
    while (cap < need) cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (newptr == NULL) abort();
    cache = newptr;
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();   /* sequence is 1‑indexed */

    init_cache();

    /* Grow cache if necessary. */
    if (n > cache_cap) grow_cache((size_t)n + 1);

    /* Compute terms up to n. */
    while (cache_len < (size_t)n) {
        /* If residual has vanished, the sequence terminates.  Fill
           remaining slots with the last value and break. */
        if (Rnum == 0) {
            for (size_t i = cache_len; i < (size_t)n; ++i)
                cache[i] = prev_val;
            cache_len = (size_t)n;
            break;
        }

        /* bound = ceil( Rden / Rnum ) */
        seq_t bound = (Rden + Rnum - 1) / Rnum;
        if (bound <= prev_val) bound = prev_val + 1;

        /* Store next term. */
        cache[cache_len] = bound;
        prev_val = bound;
        ++cache_len;

        /* Update residual: R' = R - 1/bound */
        seq_t new_num = Rnum * bound - Rden;
        seq_t new_den = Rden * bound;
        seq_t g = gcd(new_num, new_den);
        Rnum = new_num / g;
        Rden = new_den / g;
    }

    return cache[n - 1];
}
