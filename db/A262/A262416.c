/*
 * A262416  Number of (n+1) X (4+1) 0..1 arrays with each row divisible by 3
 *          and column not divisible by 3, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1)=0, a(2)=114, a(3)=1260, a(4)=18228, a(5)=200880, a(6)=2353338, a(7)=25901100;
   For n>7:
     a(n) = 11*a(n-1) + 48*a(n-2) - 528*a(n-3) - 579*a(n-4)
            + 6369*a(n-5) + 1612*a(n-6) - 17732*a(n-7) */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static idx_t cache_size = 0;     /* number of cached terms */

/* Grow the cache to hold at least n terms (n>=1). */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache && n > 0)
        abort(); /* allocation failed */

    cache = new_cache;

    /* Initial values a(1)..a(7) */
    static const seq_t init[7] = {
        0ULL,          /* a(1) */
        114ULL,        /* a(2) */
        1260ULL,       /* a(3) */
        18228ULL,      /* a(4) */
        200880ULL,     /* a(5) */
        2353338ULL,    /* a(6) */
        25901100ULL    /* a(7) */
    };

    /* Compute and store new entries */
    for (idx_t i = cache_size; i < n; ++i) {
        if (i < 7) {
            cache[i] = init[i];
        } else {
            /* Use 0‑based indices:
               cache[i]   = a(i+1)
               cache[i-1] = a(i)
               cache[i-2] = a(i-1)
               ...
               cache[i-7] = a(i-6)          */
            seq_t pos =  (seq_t)11 * cache[i-1] + (seq_t)48 * cache[i-2]
                      + (seq_t)6369 * cache[i-5] + (seq_t)1612 * cache[i-6];
            seq_t neg =  (seq_t)528 * cache[i-3] + (seq_t)579 * cache[i-4]
                      + (seq_t)17732 * cache[i-7];
            cache[i] = pos - neg;
        }
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for n=0, but return 0 for safety */

    ensure_cache(n);
    return cache[n-1];   /* cache was 0‑based */
}
