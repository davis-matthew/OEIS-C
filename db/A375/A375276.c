/*
 * A375276  Expansion of 1/sqrt(1 - 4*x - 8*x^3 + 4*x^4).
 * https://oeis.org/A375276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n*a(n) = 2*(2*n-1)*a(n-1) + 4*(2*n-3)*a(n-3) - 4*(n-2)*a(n-4)
 *            a(0)=1, a(1)=2, a(2)=6, a(3)=24
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;   /* highest index that has been computed */

static void ensure_cache(idx_t n)
{
    if (cache == NULL)    /* first call – allocate and seed the base values */
    {
        size_t init_cap = 4;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)24;
        cache_max = 3;
    }
    if (n <= cache_max) return;          /* already computed */

    /* enlarge the allocated block if needed */
    size_t cap = cache_max + 1;
    while (cap <= n) cap <<= 1;          /* double until we fit n */
    if (cap > (cache_max + 1))
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));

    /* compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i < 4)                     /* safety for tiny n (shouldn't happen) */
        {
            switch(i){
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)2; break;
                case 2: cache[i] = (seq_t)6; break;
                case 3: cache[i] = (seq_t)24; break;
            }
        }
        else
        {
            seq_t term = 0;
            /* 2*(2*i-1)*a(i-1) */
            term += ((seq_t)(2*(2*i-1))) * cache[i-1];
            /* 4*(2*i-3)*a(i-3) */
            term += ((seq_t)(4*(2*i-3))) * cache[i-3];
            /* 4*(i-2)*a(i-4) */
            term -= ((seq_t)(4*(i-2))) * cache[i-4];
            cache[i] = term / (seq_t)i;    /* exact division */
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
