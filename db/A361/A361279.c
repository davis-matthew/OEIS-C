/*
 * A361279  Expansion of e.g.f. exp(x * (1+x)^3).
 * https://oeis.org/A361279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A361279: a(0)=1, a(1)=1, a(2)=7, a(3)=37.
   Recurrence (O‑EIS) for n≥4 :
      a(n) = a(n-1) + 6*(n-1)*a(n-2)
           + 9*(n-1)*(n-2)*a(n-3)
           + 4*(n-1)*(n-2)*(n-3)*a(n-4)   */

/* seq_t and idx_t are defined elsewhere in the build environment.
   The function is memoised in a statically allocated array that grows
   as needed.  It works for arbitrary‑precision seq_t as well as for
   native integral types. */

#include <stdlib.h>

/* static cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* number of allocated slots */
static idx_t computed = 0;    /* number of values already computed */

/* grow the cache to hold at least 'needed' elements */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = (needed > 16 ? needed * 2 : 16);
    seq_t *new_cache = NULL;

    if (cache_cap == 0)
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    if (new_cache == NULL)
        exit(1);  /* out of memory */

    cache = new_cache;
    cache_cap = new_cap;
}

/* public entry point – returns a(n) */
seq_t a(idx_t n)
{
    if (n < computed)
        return cache[n];

    grow_cache(n + 1);

    for (idx_t k = computed; k <= n; ++k) {
        switch (k) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)1; break;
            case 2: cache[2] = (seq_t)7; break;
            case 3: cache[3] = (seq_t)37; break;
            default:
                /* recurrence for k ≥ 4 */
                seq_t t = cache[k-1];
                t += (seq_t)6 * (seq_t)(k-1) * cache[k-2];
                t += (seq_t)9 * (seq_t)(k-1) * (seq_t)(k-2) * cache[k-3];
                t += (seq_t)4 * (seq_t)(k-1) * (seq_t)(k-2) * (seq_t)(k-3) * cache[k-4];
                cache[k] = t;
                break;
        }
    }
    computed = n + 1;
    return cache[n];
}
