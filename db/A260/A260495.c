/*
 * A260495  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000011 or 00001011.
 * https://oeis.org/A260495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) +6*a(n-4) -7*a(n-6) -18*a(n-7) -10*a(n-8) *
 *            +5*a(n-9) +7*a(n-10) +16*a(n-11) +6*a(n-12) * -4*a(n-13)
 *            -3*a(n-15) -2*a(n-16) +a(n-17) for n>19 * (empirical recurrence;
 *            base values for n=1..19 are hard‑coded)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 64, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*----------------------------------------------------------*/
/*  Memoised linear recurrence for A260495                   */
/*----------------------------------------------------------*/
static seq_t *cache = NULL;            /* 1‑based array of values */
static idx_t  last_computed = 0;       /* last index that has a value */
static size_t cache_capacity = 0;      /* allocated size for cache array */

/* Initialise the cache with the first 19 terms (base values). */
static void init_cache(void)
{
    if (cache != NULL)
        return;                       /* already initialised */

    /* Allocate space for indices 0..19 (20 elements, index 0 unused). */
    cache_capacity = 20;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    if (cache == NULL)                /* allocation failure – will crash later */
        return;

    cache[0] = (seq_t)0;              /* unused */

    /* base values a(1) .. a(19) from the OEIS definition */
    cache[1]  = (seq_t)83;
    cache[2]  = (seq_t)136;
    cache[3]  = (seq_t)347;
    cache[4]  = (seq_t)683;
    cache[5]  = (seq_t)942;
    cache[6]  = (seq_t)2115;
    cache[7]  = (seq_t)4393;
    cache[8]  = (seq_t)6800;
    cache[9]  = (seq_t)13290;
    cache[10] = (seq_t)27885;
    cache[11] = (seq_t)47140;
    cache[12] = (seq_t)86478;
    cache[13] = (seq_t)177017;
    cache[14] = (seq_t)317762;
    cache[15] = (seq_t)571813;
    cache[16] = (seq_t)1132672;
    cache[17] = (seq_t)2104849;
    cache[18] = (seq_t)3796781;
    cache[19] = (seq_t)7316331;

    last_computed = 19;
}

/* Resize the cache array to at least new_cap elements. */
static void ensure_capacity(size_t new_cap)
{
    if (cache_capacity >= new_cap)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp != NULL) {
        cache = tmp;
        cache_capacity = new_cap;
    }
}

/* Return a(n) for n >= 1 (undefined for n == 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* 0 is outside the sequence range */

    init_cache();

    /* If we already have the value, just return it. */
    if (n <= last_computed)
        return cache[n];

    /* Extend the cache to accommodate up to n. */
    ensure_capacity((size_t)n + 1);

    /* Compute terms from last_computed+1 up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 19) {                 /* safety: should not happen */
            continue;
        }

        /* Compute the recurrence using only additions/subtractions of
         * unsigned values – the expression is guaranteed to be non‑negative. */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* Positive contributions */
        pos += (seq_t)6 * cache[i - 3];
        pos += (seq_t)6 * cache[i - 4];
        pos += (seq_t)5 * cache[i - 9];
        pos += (seq_t)7 * cache[i -10];
        pos += (seq_t)16 * cache[i -11];
        pos += (seq_t)6 * cache[i -12];
        pos += cache[i -17];

        /* Negative contributions */
        neg += (seq_t)7 * cache[i -6];
        neg += (seq_t)18 * cache[i -7];
        neg += (seq_t)10 * cache[i -8];
        neg += (seq_t)4 * cache[i -13];
        neg += (seq_t)3 * cache[i -15];
        neg += (seq_t)2 * cache[i -16];

        cache[i] = pos - neg;          /* pos >= neg for all valid i */
    }

    last_computed = n;
    return cache[n];
}
